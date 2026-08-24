use std::ffi::{CString, c_char, c_int};

#[expect(non_camel_case_types, reason = "bindgen")]
mod bindings {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

pub trait DoomCallbacks {
    fn new() -> Self
    where
        Self: Sized;
    fn init(&mut self);
    fn on_draw(&mut self, buf: &[u8; 320 * 200]);
    // if there was an event, (pressed, code)
    fn get_key_event(&mut self) -> Option<(bool, u8)>;
    fn sleep(&mut self, ms: u32);
    fn get_elapsed(&mut self) -> core::time::Duration;
    fn set_window_title(&mut self, title: &str);
}

mod doom_sys {
    use std::{
        cell::Cell,
        ffi::{CStr, c_char, c_int},
        ptr, slice, thread,
        time::Duration,
    };

    use crate::doom::{DoomCallbacks, bindings};

    thread_local! {
        pub static CALLBACKS: Cell<Option<*mut dyn DoomCallbacks>> = Cell::new(None);
    }

    fn with_callbacks<F, T>(f: F) -> Option<T>
    where
        F: FnOnce(&mut dyn DoomCallbacks) -> T,
    {
        let callbacks = CALLBACKS.get();
        if let Some(callbacks) = callbacks && !callbacks.is_null() {
            let callbacks = unsafe { &mut *callbacks };
            Some(f(callbacks))
        } else {
            None
        }
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_Init() {
        with_callbacks(|callbacks| {
            callbacks.init();
        });
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_DrawFrame() {
        let pointer = unsafe { bindings::DG_ScreenBuffer };
        if pointer.is_null() {
            return;
        }
        let buf = unsafe { pointer.cast::<[u8; 320 * 200]>().as_ref() };
        if buf.is_none() {
            return;
        }
        // let state = unsafe { super::bindings::doomsat_State() };
        // dbg!(state.gametic);

        with_callbacks(|callbacks| callbacks.on_draw(buf.unwrap()));
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_SleepMs(ms: u32) {
        with_callbacks(|callbacks| callbacks.sleep(ms));
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_GetTicksMs() -> u32 {
        with_callbacks(|callbacks| callbacks.get_elapsed().as_millis() as u32).unwrap_or(0)
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_GetKey(pressed: *mut c_int, key: *mut u8) -> c_int {
        let event = with_callbacks(|callbacks| callbacks.get_key_event());
        let Some(Some((event_pressed, event_key))) = event else { return 0 };
        unsafe {
            *pressed = event_pressed as c_int;
            *key = event_key as u8;
        }
        return 1;
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_SetWindowTitle(title: *const c_char) {
        let title = unsafe { CStr::from_ptr(title) }.to_str();
        if let Ok(title) = title {
            with_callbacks(|callbacks| callbacks.set_window_title(title));
        }
    }
}

struct Doom<T: DoomCallbacks> {
    callbacks: T,
}
impl<T: DoomCallbacks> Doom<T> {
    pub fn new(callbacks: T) -> Self {
        Self { callbacks }
    }
    pub fn init(&mut self) {
        doom_sys::CALLBACKS.with(|callbacks| {
            let ptr: *mut dyn DoomCallbacks = &raw mut self.callbacks;
            let ptr: *mut (dyn DoomCallbacks + 'static) = unsafe { std::mem::transmute(ptr) };
            callbacks.set(Some(ptr))
        });
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

    unsafe { bindings::doomgeneric_Create(argv_ptrs.len() as c_int, argv_ptrs.as_mut_ptr()) };

    std::mem::forget(argv);
    std::mem::forget(argv_ptrs);
}

pub fn tick() {
    unsafe { bindings::doomgeneric_Tick() };
}
