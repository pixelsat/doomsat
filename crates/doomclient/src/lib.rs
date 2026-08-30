//! doomclient bindings. significantly lighterweight than doomstm
//! since we can make some nice assumptions, particular that we're
//! running on a relatively capable std-enabled host.

use core::ffi::{c_char, c_int};
use std::time::Instant;

use minifb::{InputCallback, Window, WindowOptions};
use shared::doom_serde::ArchivedDoomsatState;
use shared::doom_sys;

use crate::sys::STATE;

mod sys;

const ARGC: c_int = 3;

static mut ARGV: [*mut c_char; ARGC as usize] = [
    c"doomclient".as_ptr().cast_mut(),
    c"-iwad".as_ptr().cast_mut(),
    c"../../wad/doom1.wad".as_ptr().cast_mut(),
    // c"-skill".as_ptr().cast_mut(),
    // c"5".as_ptr().cast_mut(),
    // c"-warp".as_ptr().cast_mut(),
    // c"1".as_ptr().cast_mut(),
    // c"9".as_ptr().cast_mut(),
];

pub struct Doom;
impl Doom {
    pub fn create<H: InputCallback + 'static>(handler: H) -> Self {
        let mut window = Window::new("doomsat", 640, 400, WindowOptions::default()).unwrap();
        window.set_input_callback(Box::new(handler));

        STATE.with_borrow_mut(|state| {
            state.window = Some(window);
            state.init_time = Some(Instant::now());
        });

        unsafe {
            doom_sys::doomgeneric_Create(ARGC, core::ptr::addr_of_mut!(ARGV).cast::<*mut c_char>())
        };

        Doom
    }
    pub fn tick(&mut self) {
        unsafe { doom_sys::doomgeneric_Tick() };
    }
    pub fn draw(&mut self, state: &ArchivedDoomsatState) {
        unsafe { doom_sys::doomsat_Draw(state.into()) };
    }
}
