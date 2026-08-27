use core::cell::RefCell;
use core::ffi::{c_char, c_int};
use std::ffi::CStr;
use std::time::Instant;

use minifb::Window;
use shared::doom_sys;

pub(crate) struct DoomclientState {
    pub(crate) window: Option<Window>,
    pub(crate) init_time: Option<Instant>,
}

impl DoomclientState {
    const fn new() -> Self {
        Self {
            window: None,
            init_time: None,
        }
    }
}

thread_local! {
    pub(crate) static STATE: RefCell<DoomclientState> = const { RefCell::new(DoomclientState::new()) };
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_Init() {
    println!("[doomclient::sys] DG_Init called");
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_DrawFrame() {
    let Some(buf) = (unsafe {
        doom_sys::DG_ScreenBuffer
            .cast::<[u32; 640 * 400]>()
            .as_ref()
    }) else {
        return;
    };

    STATE.with_borrow_mut(|state| {
        if let Some(window) = state.window.as_mut() {
            let _ = window.update_with_buffer(buf, 640, 400);
        }
    });
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_SleepMs(ms: u32) {
    std::thread::sleep(std::time::Duration::from_millis(ms.into()));
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_GetTicksMs() -> u32 {
    STATE.with_borrow(|state| {
        state
            .init_time
            .as_ref()
            .map_or(0, |init_time| init_time.elapsed().as_millis() as u32)
    })
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_GetKey(_pressed: *mut c_int, _key: *mut u8) -> c_int {
    return 0;
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_SetWindowTitle(title: *const c_char) {
    if title.is_null() {
        return;
    }

    let title = unsafe { CStr::from_ptr(title) }.to_string_lossy();
    STATE.with_borrow_mut(|state| {
        if let Some(window) = state.window.as_mut() {
            window.set_title(&title);
        }
    });
}
