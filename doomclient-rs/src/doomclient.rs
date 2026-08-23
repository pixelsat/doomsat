use std::ffi::{self, CString, c_char, c_int};

#[repr(C)]
struct doomsat_thing {
    x: ffi::c_int,
    y: ffi::c_int,
    z: ffi::c_int,
    angle: u32,
    frame: ffi::c_int,
    flags: ffi::c_int,
}
#[repr(C)]
struct doomsat_state {
    thinkercap_length: ffi::c_int,
    thinkercap: *const doomsat_thing,
}

mod doomclient_unsafe {
    use std::{
        cell::RefCell,
        collections::VecDeque,
        ffi::{CStr, c_char, c_int},
        slice,
        sync::OnceLock,
        thread,
        time::{Duration, Instant},
    };

    use minifb::{InputCallback, Key, Window, WindowOptions};

    static INIT_TIME: OnceLock<Instant> = OnceLock::new();
    thread_local! {
        static WINDOW: RefCell<Option<Window>> = const { RefCell::new(None) };
        static KEY_QUEUE: RefCell<VecDeque<(c_int, u8)>> = const { RefCell::new(VecDeque::new()) };
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

    unsafe extern "C" {
        pub fn doomgeneric_Create(argc: c_int, argv: *mut *mut c_char);
        pub fn doomgeneric_Tick();
        pub fn doomsat_State() -> super::doomsat_state;

        static mut DG_ScreenBuffer: *mut u32;
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_Init() {
        INIT_TIME.set(Instant::now()).ok();
        WINDOW.with(|window| {
            let mut doom_window = Window::new("Doom", 320, 200, WindowOptions::default())
                .expect("failed to create window");
            doom_window.set_input_callback(Box::new(DoomInput));
            *window.borrow_mut() = Some(doom_window);
        });
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_DrawFrame() {
        let state = unsafe { doomsat_State() };
        dbg!(state.thinkercap_length);

        let pixels = unsafe { slice::from_raw_parts(DG_ScreenBuffer, 320 * 200) };

        WINDOW.with(|window| {
            window
                .borrow_mut()
                .as_mut()
                .expect("window not initialized")
                .update_with_buffer(pixels, 320, 200)
                .expect("failed to draw frame");
        });
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_SleepMs(ms: u32) {
        thread::sleep(Duration::from_millis(ms.into()));
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_GetTicksMs() -> u32 {
        INIT_TIME
            .get()
            .map(|t| t.elapsed().as_millis() as u32)
            .unwrap_or(0)
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_GetKey(pressed: *mut c_int, key: *mut u8) -> c_int {
        if pressed.is_null() || key.is_null() {
            return 0;
        }

        let Some((event_pressed, event_key)) =
            KEY_QUEUE.with(|queue| queue.borrow_mut().pop_front())
        else {
            return 0;
        };

        unsafe {
            *pressed = event_pressed;
            *key = event_key;
        }

        1
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_SetWindowTitle(title: *const c_char) {
        let title = unsafe { CStr::from_ptr(title) };
        println!("set title: {:?}", title);
    }
}

pub fn create(args: &[String]) {
    let argv = args
        .iter()
        .map(|arg| CString::new(arg.as_str()).expect("arguments cannot contain NUL bytes"))
        .collect::<Vec<_>>();
    let mut argv_ptrs = argv
        .iter()
        .map(|arg| arg.as_ptr() as *mut c_char)
        .collect::<Vec<_>>();

    unsafe {
        doomclient_unsafe::doomgeneric_Create(argv_ptrs.len() as c_int, argv_ptrs.as_mut_ptr())
    };

    std::mem::forget(argv);
    std::mem::forget(argv_ptrs);
}

pub fn tick() {
    unsafe { doomclient_unsafe::doomgeneric_Tick() };
}
