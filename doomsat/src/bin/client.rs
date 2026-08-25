use crossterm::{
    event::{
        Event, KeyCode, KeyEventKind, KeyModifiers, KeyboardEnhancementFlags,
        PushKeyboardEnhancementFlags, PopKeyboardEnhancementFlags, poll, read,
    },
    execute,
    terminal::{disable_raw_mode, enable_raw_mode},
};
use doomsat::doom::{Doom, DoomCallbacks, serialize_state};
use std::{
    collections::HashMap,
    io::{Write, stdout},
    os::unix::net::UnixStream,
    path::PathBuf,
    sync::mpsc::{self, Receiver, Sender},
    thread,
    time::{Duration, Instant},
};

// Without the kitty keyboard protocol terminals never send key-up, so a key
// counts as released once it stops auto-repeating for this long.
const RELEASE_TIMEOUT: Duration = Duration::from_millis(150);

struct DoomClient {
    initial_time: Instant,
    key_queue: Receiver<(bool, u8)>,
}

fn input_thread(tx: Sender<(bool, u8)>) {
    let mut held: HashMap<u8, Instant> = HashMap::new();
    loop {
        if poll(Duration::from_millis(10)).unwrap_or(false) {
            if let Ok(Event::Key(ev)) = read() {
                if ev.code == KeyCode::Char('c') && ev.modifiers.contains(KeyModifiers::CONTROL) {
                    cleanup();
                    std::process::exit(0);
                }
                let key = if ev.modifiers.contains(KeyModifiers::CONTROL) {
                    0xa2 // use
                } else if let Some(key) = doom_key(ev.code) {
                    key
                } else {
                    continue;
                };
                match ev.kind {
                    KeyEventKind::Release => {
                        held.remove(&key);
                        let _ = tx.send((false, key));
                    }
                    _ => {
                        if held.insert(key, Instant::now()).is_none() {
                            let _ = tx.send((true, key));
                        }
                    }
                }
            }
        }
        let now = Instant::now();
        held.retain(|&key, last| {
            let alive = now.duration_since(*last) < RELEASE_TIMEOUT;
            if !alive {
                let _ = tx.send((false, key));
            }
            alive
        });
    }
}

fn cleanup() {
    let _ = execute!(stdout(), PopKeyboardEnhancementFlags);
    let _ = disable_raw_mode();
}

impl DoomCallbacks for DoomClient {
    fn new() -> Self {
        enable_raw_mode().expect("failed to enable raw mode");
        let _ = execute!(
            stdout(),
            PushKeyboardEnhancementFlags(KeyboardEnhancementFlags::REPORT_EVENT_TYPES)
        );
        let (tx, key_queue) = mpsc::channel();
        thread::spawn(move || input_thread(tx));
        DoomClient {
            initial_time: Instant::now(),
            key_queue,
        }
    }
    fn on_draw(&mut self, _buf: &[u32; 320 * 200]) {}
    fn get_key_event(&mut self) -> Option<(bool, u8)> {
        self.key_queue.try_recv().ok()
    }
    fn sleep(&mut self, ms: u32) {
        thread::sleep(Duration::from_millis(ms.into()));
    }
    fn init(&mut self) {}
    fn set_window_title(&mut self, _title: &str) {}
    fn get_elapsed(&mut self) -> Duration {
        self.initial_time.elapsed()
    }
}

fn doom_key(code: KeyCode) -> Option<u8> {
    Some(match code {
        KeyCode::Char(c) if c.is_ascii() => {
            let c = c.to_ascii_lowercase() as u8;
            if c == b' ' { 0xa3 } else { c } // space fires
        }
        KeyCode::Right => 0xae,
        KeyCode::Left => 0xac,
        KeyCode::Up => 0xad,
        KeyCode::Down => 0xaf,
        KeyCode::Esc => 27,
        KeyCode::Enter => 13,
        KeyCode::Tab => 9,
        KeyCode::Backspace => 0x7f,
        KeyCode::Home => 0xc7,
        KeyCode::End => 0xcf,
        KeyCode::PageUp => 0xc9,
        KeyCode::PageDown => 0xd1,
        KeyCode::Insert => 0xd2,
        KeyCode::Delete => 0xd3,
        KeyCode::Pause => 0xff,
        KeyCode::CapsLock => 0xba,
        KeyCode::NumLock => 0xc5,
        KeyCode::ScrollLock => 0xc6,
        KeyCode::F(n @ 1..=10) => 0xbb + (n - 1),
        KeyCode::F(11) => 0xd7,
        KeyCode::F(12) => 0xd8,
        _ => return None,
    })
}

fn main() {
    let wad = PathBuf::from("../wad/doom1.wad");

    let mut client = DoomClient::new();
    let mut doom = Doom::create(
        &mut client,
        &[
            "doomclient".to_owned(),
            "-iwad".to_owned(),
            wad.to_string_lossy().into_owned(),
            "-skill".to_owned(),
            "5".to_owned(),
            "-warp".to_owned(),
            "1".to_owned(),
            "9".to_owned(),
        ],
    );
    let mut sock = UnixStream::connect("/tmp/doomsat_state.sock")
        .expect("failed to connect to Doom state receiver");
    loop {
        doom.tick();
        let state = doom.get_state();
        let bytes = serialize_state(&state).expect("failed to serialize Doom state");
        let len = u64::try_from(bytes.len()).expect("Doom state is too large");
        sock.write_all(&len.to_le_bytes()).unwrap();
        sock.write_all(&bytes).unwrap();
    }
}
