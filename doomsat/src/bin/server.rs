use doomsat::doom::{Doom, DoomCallbacks, serialize_state};
use std::{
    io::{Read, Write},
    os::unix::net::{UnixListener, UnixStream},
    path::PathBuf,
    sync::mpsc::{self, Receiver},
    thread,
    time::{Duration, Instant},
};

const SOCKET_PATH: &str = "/tmp/doomsat.sock";

struct DoomClient {
    initial_time: Instant,
    key_events: Receiver<(bool, u8)>,
}

impl DoomClient {
    fn new(key_events: Receiver<(bool, u8)>) -> Self {
        DoomClient {
            initial_time: Instant::now(),
            key_events,
        }
    }
}

impl DoomCallbacks for DoomClient {
    fn on_draw(&mut self, _buf: &[u32; 640 * 400]) {}

    fn get_key_event(&mut self) -> Option<(bool, u8)> {
        self.key_events.try_recv().ok()
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

fn read_input(mut socket: UnixStream, sender: mpsc::Sender<(bool, u8)>) {
    loop {
        let mut event = [0u8; 2];
        if socket.read_exact(&mut event).is_err() {
            return;
        }
        if sender.send((event[0] != 0, event[1])).is_err() {
            return;
        }
    }
}

fn main() {
    let _ = std::fs::remove_file(SOCKET_PATH);
    let listener = UnixListener::bind(SOCKET_PATH).expect("failed to bind Doom socket");
    let (mut socket, _) = listener.accept().expect("failed to accept Doom client");

    let input_socket = socket
        .try_clone()
        .expect("failed to clone Doom client socket");
    let (sender, key_events) = mpsc::channel();
    thread::spawn(move || read_input(input_socket, sender));

    let wad = PathBuf::from("../wad/doom1.wad");
    let mut client = DoomClient::new(key_events);
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

    loop {
        doom.tick();
        let state = doom.get_state();
        let bytes = serialize_state(&state).expect("failed to serialize Doom state");
        let len = u64::try_from(bytes.len()).expect("Doom state is too large");

        if socket.write_all(&len.to_le_bytes()).is_err() || socket.write_all(&bytes).is_err() {
            return;
        }
    }
}
