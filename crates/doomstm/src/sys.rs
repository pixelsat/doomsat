use crate::Callbacks;
use core::cell::Cell;
use core::ffi::{c_char, c_int};

pub(crate) static CALLBACKS: Callbacks = Callbacks(Cell::new(None));

#[unsafe(no_mangle)]
pub extern "C" fn DG_Init() {
    CALLBACKS.with(|c| c.log("DG_Init called"));
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_DrawFrame() {
    // noop since stm doesn't ever need to draw
    // CALLBACKS.with(|c| c.log("DG_DrawFrame called"));
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_putc_stdout(ch: c_int) {
    CALLBACKS.with(|c| c.putc_stdout(ch as u8));
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_putc_stderr(ch: c_int) {
    CALLBACKS.with(|c| c.putc_stderr(ch as u8));
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_flush_stdout() {
    CALLBACKS.with(|c| c.flush_stdout());
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_flush_stderr() {
    CALLBACKS.with(|c| c.flush_stderr());
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_SleepMs(ms: u32) {
    CALLBACKS.with(|c| c.sleep(ms));
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_GetTicksMs() -> u32 {
    CALLBACKS
        .with(|c| c.get_elapsed().as_millis() as u32)
        .unwrap_or(0)
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_GetKey(pressed: *mut c_int, key: *mut u8) -> c_int {
    let event = CALLBACKS.with(|c| c.get_key_event());
    let Some(Some((event_pressed, event_key))) = event else {
        return 0;
    };
    unsafe {
        *pressed = event_pressed as c_int;
        *key = event_key as u8;
    }
    return 1;
}

#[unsafe(no_mangle)]
pub extern "C" fn DG_SetWindowTitle(_title: *const c_char) {
    CALLBACKS.with(|c| c.log("window title set"));
}
