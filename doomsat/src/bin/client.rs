use std::{cell::RefCell, collections::VecDeque, sync::OnceLock, time::Instant};
use minifb::{InputCallback, Key, Window, WindowOptions};
use crate::doom::DoomsatCallbacks;

struct DoomClient {
    initial_time: Instant,
    window: Window,
    key_queue: VecDeque<(bool, u8)>,
}
impl DoomsatCallbacks for DoomClient {
    fn new() -> Self {
        let window = Window::new("Doom", 320, 200, WindowOptions::default())
            .expect("failed to create window");
        window.set_input_callback(Box::new(DoomInput));
        DoomClient {
            initial_time: Instant::now(),
            window,
            key_queue: VecDeque::new(),
        }
    }
}

struct DoomInput;

impl InputCallback for DoomInput {
    fn add_char(&mut self, _character: u32) {}

    fn set_key_state(&mut self, key: Key, pressed: bool) {
        if let Some(key) = doom_key(key) {
            KEY_QUEUE.with(|queue| {
                queue.borrow_mut().push_back((pressed as c_int, key));
            });
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
        Key::Space => 0xa2,
        Key::LeftCtrl | Key::RightCtrl => 0xa3,
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

