#![no_std]
//! doomstm bindings. notably intended to work on both stm and host, so
//! a trait is used to abstract over the surface that std implements on host
//! but rtic/hal implement on stm.

use core::cell::Cell;
use core::ffi::{c_char, c_int};
use core::mem;

use shared::doom_serde::DoomsatState;
use shared::doom_sys;

use crate::allocator::setup_zones;

mod sys;
pub mod allocator;

pub trait DoomCallbacks {
    fn log(&mut self, string: &str);
    fn sleep(&mut self, ms: u32);
    fn get_elapsed(&mut self) -> core::time::Duration;
    fn get_key_event(&mut self) -> Option<(bool, u8)>;
}

#[repr(transparent)]
pub struct Callbacks(Cell<Option<*mut dyn DoomCallbacks>>);
impl Callbacks {
    fn set(&self, callbacks: Option<*mut dyn DoomCallbacks>) {
        self.0.set(callbacks);
    }
    fn replace(&self, callbacks: Option<*mut dyn DoomCallbacks>) -> Option<*mut dyn DoomCallbacks> {
        self.0.replace(callbacks)
    }
    fn with<F, T>(&self, f: F) -> Option<T>
    where
        F: FnOnce(&mut dyn DoomCallbacks) -> T,
    {
        let callbacks = self.0.get();
        if let Some(callbacks) = callbacks {
            let callbacks = unsafe { &mut *callbacks };
            Some(f(callbacks))
        } else {
            None
        }
    }
}
// SAFETY: only ever used in single-core contexts.
unsafe impl Send for Callbacks {}
unsafe impl Sync for Callbacks {}

const ARGC: c_int = 8;

static mut ARGV: [*mut c_char; ARGC as usize] = [
    c"doomstm".as_ptr().cast_mut(),
    c"-iwad".as_ptr().cast_mut(),
    c"doomsat.wad".as_ptr().cast_mut(),
    c"-skill".as_ptr().cast_mut(),
    c"5".as_ptr().cast_mut(),
    c"-warp".as_ptr().cast_mut(),
    c"1".as_ptr().cast_mut(),
    c"9".as_ptr().cast_mut(),
];

pub struct Doom<'a, T: DoomCallbacks> {
    callbacks: &'a mut T,
    _wad: &'a [u8],
}
struct Restore(Option<*mut (dyn DoomCallbacks + 'static)>);
impl Drop for Restore {
    fn drop(&mut self) {
        sys::CALLBACKS.set(self.0);
    }
}
impl<'a, T: DoomCallbacks> Doom<'a, T> {
    pub fn create(callbacks: &'a mut T, wad: &'a [u8], dtcm_zone: &'static mut [u8], sram_zone: &'static mut [u8]) -> Self {
        let mut doom = Self { callbacks, _wad: wad };
        let _restore = doom.setup();

        setup_zones(dtcm_zone, sram_zone);

        unsafe {
            doom_sys::doomgeneric_Create(
                ARGC,
                core::ptr::addr_of_mut!(ARGV).cast::<*mut c_char>(),
                wad.as_ptr(),
                wad.len(),
            )
        };

        drop(_restore);

        doom
    }
    fn setup(&mut self) -> Restore {
        let ptr: *mut (dyn DoomCallbacks + 'a) = &raw mut *self.callbacks;
        let ptr: *mut (dyn DoomCallbacks + 'static) = unsafe { mem::transmute(ptr) };
        Restore(sys::CALLBACKS.replace(Some(ptr)))
    }
    pub fn tick(&mut self) {
        let _restore = self.setup();
        unsafe { doom_sys::doomgeneric_Tick() };
    }
    pub fn get_state(&mut self) -> DoomsatState {
        unsafe { doom_sys::doomsat_GetState() }.into()
    }
    pub fn with_screen<F>(&mut self, f: F)
        where F: FnOnce(&[u32; 640 * 400])
    {
        let pointer = unsafe { doom_sys::DG_ScreenBuffer };
        if pointer.is_null() {
            return;
        }
        let buf = unsafe { pointer.cast::<[u32; 640 * 400]>().as_ref() };
        if buf.is_none() {
            return;
        }
        let buf = buf.unwrap();
        f(buf);
    }
}
