use std::{
    io::{Read, Write},
    os::unix::net::UnixStream,
    path::PathBuf,
    sync::mpsc::{self, Sender},
    time::{Duration, Instant},
};

use doomsat::doom::{Doom, DoomCallbacks, deserialize_state};
use minifb::{InputCallback, Key, Window, WindowOptions};

const MAX_STATE_SIZE: usize = 64 * 1024 * 1024;
const SOCKET_PATH: &str = "/tmp/doomsat.sock";

struct DoomStub {
    initial_time: Instant,
}
impl DoomStub {
    fn new() -> Self {
        DoomStub {
            initial_time: Instant::now(),
        }
    }
}
impl DoomCallbacks for DoomStub {
    fn on_draw(&mut self, _buf: &[u32; 640 * 400]) {
        dbg!("on_draw");
    }
    fn get_key_event(&mut self) -> Option<(bool, u8)> {
        dbg!("get_key_event");
        None
    }
    fn sleep(&mut self, ms: u32) {
        std::thread::sleep(std::time::Duration::from_millis(ms.into()));
    }
    fn init(&mut self) {}
    fn set_window_title(&mut self, _title: &str) {}
    fn get_elapsed(&mut self) -> core::time::Duration {
        self.initial_time.elapsed()
    }
}

struct DoomInput {
    key_events: Sender<(bool, u8)>,
}
impl InputCallback for DoomInput {
    fn add_char(&mut self, _character: u32) {}

    fn set_key_state(&mut self, key: Key, pressed: bool) {
        if let Some(key) = doom_key(key) {
            let _ = self.key_events.send((pressed, key));
        }
    }
}

fn connect(window: &mut Window) -> Option<UnixStream> {
    let mut waiting = false;

    while window.is_open() {
        match UnixStream::connect(SOCKET_PATH) {
            Ok(socket) => return Some(socket),
            Err(error) => {
                if !waiting {
                    eprintln!("waiting for Doom server: {error}");
                    waiting = true;
                }
            }
        }

        window.update();
        std::thread::sleep(Duration::from_millis(50));
    }

    None
}

fn main() {
    let wad = PathBuf::from("../wad/doom1.wad");
    let mut client = DoomStub::new();
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

    let (sender, receiver) = mpsc::channel();
    let mut window =
        Window::new("Doom", 640, 400, WindowOptions::default()).expect("failed to create window");
    window.set_input_callback(Box::new(DoomInput { key_events: sender }));

    let mut buf = Vec::new();
    while window.is_open() {
        let Some(mut socket) = connect(&mut window) else {
            return;
        };

        loop {
            let mut len = [0; size_of::<u64>()];
            if socket.read_exact(&mut len).is_err() {
                break;
            }

            let len = usize::try_from(u64::from_le_bytes(len)).expect("state is too large");
            assert!(len <= MAX_STATE_SIZE, "state is too large");
            buf.resize(len, 0);
            if socket.read_exact(&mut buf).is_err() {
                break;
            }

            let Ok(state) = deserialize_state(&buf) else {
                continue;
            };
            doom.draw_state(state);
            let mut window_error = None;
            doom.with_screen(|screen| {
                window_error = window.update_with_buffer(screen, 640, 400).err();
            });
            if window_error.is_some() || !window.is_open() {
                return;
            }

            let mut disconnected = false;
            while let Ok((pressed, key)) = receiver.try_recv() {
                if socket.write_all(&[pressed as u8, key]).is_err() {
                    disconnected = true;
                    break;
                }
            }
            if disconnected {
                break;
            }
        }
    }
}

fn doom_key(key: Key) -> Option<u8> {
    Some(match key {
        Key::A => b'a',
        Key::B => b'b',
        Key::C => b'c',
        Key::D => b'd',
        Key::E => b'e',
        Key::F => b'f',
        Key::G => b'g',
        Key::H => b'h',
        Key::I => b'i',
        Key::J => b'j',
        Key::K => b'k',
        Key::L => b'l',
        Key::M => b'm',
        Key::N => b'n',
        Key::O => b'o',
        Key::P => b'p',
        Key::Q => b'q',
        Key::R => b'r',
        Key::S => b's',
        Key::T => b't',
        Key::U => b'u',
        Key::V => b'v',
        Key::W => b'w',
        Key::X => b'x',
        Key::Y => b'y',
        Key::Z => b'z',
        Key::Key0 => b'0',
        Key::Key1 => b'1',
        Key::Key2 => b'2',
        Key::Key3 => b'3',
        Key::Key4 => b'4',
        Key::Key5 => b'5',
        Key::Key6 => b'6',
        Key::Key7 => b'7',
        Key::Key8 => b'8',
        Key::Key9 => b'9',
        Key::Right => 0xae,
        Key::Left => 0xac,
        Key::Up => 0xad,
        Key::Down => 0xaf,
        Key::Comma => 0xa0,
        Key::Period => 0xa1,
        Key::Space => 0xa3,
        // ...ok i switched ctrl & space bc... cmon
        Key::LeftCtrl | Key::RightCtrl => 0xa2,
        Key::Escape => 27,
        Key::Enter | Key::NumPadEnter => 13,
        Key::Tab => 9,
        Key::F1 => 0xbb,
        Key::F2 => 0xbc,
        Key::F3 => 0xbd,
        Key::F4 => 0xbe,
        Key::F5 => 0xbf,
        Key::F6 => 0xc0,
        Key::F7 => 0xc1,
        Key::F8 => 0xc2,
        Key::F9 => 0xc3,
        Key::F10 => 0xc4,
        Key::F11 => 0xd7,
        Key::F12 => 0xd8,
        Key::Backspace => 0x7f,
        Key::Pause => 0xff,
        Key::Equal => b'=',
        Key::Minus => b'-',
        Key::LeftShift | Key::RightShift => 0xb6,
        Key::LeftAlt | Key::RightAlt => 0xb8,
        Key::CapsLock => 0xba,
        Key::NumLock => 0xc5,
        Key::ScrollLock => 0xc6,
        Key::Home => 0xc7,
        Key::End => 0xcf,
        Key::PageUp => 0xc9,
        Key::PageDown => 0xd1,
        Key::Insert => 0xd2,
        Key::Delete => 0xd3,
        Key::Apostrophe => b'\'',
        Key::Backquote => b'`',
        Key::Backslash => b'\\',
        Key::LeftBracket => b'[',
        Key::RightBracket => b']',
        Key::Semicolon => b';',
        Key::Slash | Key::NumPadSlash => b'/',
        Key::NumPadAsterisk => b'*',
        Key::NumPadMinus => b'-',
        Key::NumPadPlus => b'+',
        Key::NumPad0 | Key::NumPadDot => 0,
        Key::NumPad1 => 0xcf,
        Key::NumPad2 => 0xaf,
        Key::NumPad3 => 0xd1,
        Key::NumPad4 => 0xac,
        Key::NumPad5 => b'5',
        Key::NumPad6 => 0xae,
        Key::NumPad7 => 0xc7,
        Key::NumPad8 => 0xad,
        Key::NumPad9 => 0xc9,
        _ => return None,
    })
}
