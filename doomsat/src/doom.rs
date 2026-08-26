use std::ffi::{CString, c_char, c_int};

use bincode::Options;
use serde::{Deserialize, Serialize};

#[expect(non_camel_case_types, reason = "bindgen")]
mod bindings {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

pub trait DoomCallbacks {
    fn init(&mut self);
    fn on_draw(&mut self, buf: &[u32; 640 * 400]);
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
        if let Some(callbacks) = callbacks
            && !callbacks.is_null()
        {
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
        let buf = unsafe { pointer.cast::<[u32; 640 * 400]>().as_ref() };
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
    pub extern "C" fn DG_SetWindowTitle(title: *const c_char) {
        let title = unsafe { CStr::from_ptr(title) }.to_str();
        if let Ok(title) = title {
            with_callbacks(|callbacks| callbacks.set_window_title(title));
        }
    }
}

pub struct Doom<'a, T: DoomCallbacks> {
    callbacks: &'a mut T,
}
struct Restore(Option<*mut (dyn DoomCallbacks + 'static)>);
impl Drop for Restore {
    fn drop(&mut self) {
        doom_sys::CALLBACKS.set(self.0);
    }
}
impl<'a, T: DoomCallbacks> Doom<'a, T> {
    pub fn create(callbacks: &'a mut T, args: &[String]) -> Self {
        let mut doom = Self { callbacks };
        let _restore = doom.setup();
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

        drop(_restore);

        doom
    }
    fn setup(&mut self) -> Restore {
        let ptr: *mut (dyn DoomCallbacks + 'a) = &raw mut *self.callbacks;
        let ptr: *mut (dyn DoomCallbacks + 'static) = unsafe { std::mem::transmute(ptr) };
        Restore(doom_sys::CALLBACKS.replace(Some(ptr)))
    }
    pub fn tick(&mut self) {
        let _restore = self.setup();
        unsafe { bindings::doomgeneric_Tick() };
    }
    pub fn get_state(&mut self) -> DoomsatState {
        unsafe { bindings::doomsat_GetState() }.into()
    }
    pub fn draw_state(&mut self, state: DoomsatState) {
        unsafe { bindings::doomsat_Draw(state.into()) }
    }
    pub fn with_screen<F>(&mut self, f: F)
        where F: FnOnce(&[u32; 640 * 400])
    {
        let pointer = unsafe { bindings::DG_ScreenBuffer };
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
// TODO: clean up and make idiomatic
#[derive(Serialize, Deserialize, Clone)]
pub struct DoomsatState {
    pub gamestate: ::std::os::raw::c_int,
    pub gametic: ::std::os::raw::c_int,
    pub leveltime: ::std::os::raw::c_int,
    pub paused: boolean,
    pub automapactive: boolean,
    pub menuactive: boolean,
    pub viewx: ::std::os::raw::c_int,
    pub viewy: ::std::os::raw::c_int,
    pub viewz: ::std::os::raw::c_int,
    pub viewangle: u32,
    pub player_viewz: ::std::os::raw::c_int,
    pub player_extralight: ::std::os::raw::c_int,
    pub player_fixedcolormap: ::std::os::raw::c_int,
    pub player_health: ::std::os::raw::c_int,
    pub player_armorpoints: ::std::os::raw::c_int,
    pub player_ammo: [::std::os::raw::c_int; 4usize],
    pub player_maxammo: [::std::os::raw::c_int; 4usize],
    pub player_readyweapon: ::std::os::raw::c_int,
    pub player_weaponowned: [boolean; 9usize],
    pub player_cards: [boolean; 6usize],
    pub player_frags: [::std::os::raw::c_int; 4usize],
    pub player_powers: [::std::os::raw::c_int; 6usize],
    pub player_message: u16,
    pub player_psprites: [doomsat_psprite; 2usize],
    pub sectors: Vec<doomsat_sector>,
    pub sides: Vec<doomsat_side>,
    pub lines: Vec<doomsat_line>,
    pub mobjs: Vec<doomsat_mobj>,
}

pub fn serialize_state(state: &DoomsatState) -> Result<Vec<u8>, bincode::Error> {
    bincode::DefaultOptions::new()
        .with_fixint_encoding()
        .serialize(state)
}

pub fn deserialize_state(bytes: &[u8]) -> Result<DoomsatState, bincode::Error> {
    bincode::DefaultOptions::new()
        .with_fixint_encoding()
        .with_limit(bytes.len() as u64)
        .reject_trailing_bytes()
        .deserialize(bytes)
}

impl From<doomsat_state> for DoomsatState {
    fn from(state: doomsat_state) -> Self {
        let sectors: &[doomsat_sector] =
            unsafe { std::slice::from_raw_parts(state.sectors, state.sectors_length as _) };
        let sectors = sectors.to_vec();

        let sides: &[doomsat_side] =
            unsafe { std::slice::from_raw_parts(state.sides, state.sides_length as _) };
        let sides = sides.to_vec();

        let lines: &[doomsat_line] =
            unsafe { std::slice::from_raw_parts(state.lines, state.lines_length as _) };
        let lines = lines.to_vec();

        let mobjs: &[doomsat_mobj] =
            unsafe { std::slice::from_raw_parts(state.mobjs, state.mobj_length as _) };
        let mobjs = mobjs.to_vec();

        Self {
            gamestate: state.gamestate,
            gametic: state.gametic,
            leveltime: state.leveltime,
            paused: state.paused,
            automapactive: state.automapactive,
            menuactive: state.menuactive,
            viewx: state.viewx,
            viewy: state.viewy,
            viewz: state.viewz,
            viewangle: state.viewangle,
            player_viewz: state.player_viewz,
            player_extralight: state.player_extralight,
            player_fixedcolormap: state.player_fixedcolormap,
            player_health: state.player_health,
            player_armorpoints: state.player_armorpoints,
            player_ammo: state.player_ammo,
            player_maxammo: state.player_maxammo,
            player_readyweapon: state.player_readyweapon,
            player_weaponowned: state.player_weaponowned,
            player_cards: state.player_cards,
            player_frags: state.player_frags,
            player_powers: state.player_powers,
            player_message: state.player_message,
            player_psprites: state.player_psprites,
            sectors: sectors,
            sides: sides,
            lines: lines,
            mobjs: mobjs,
        }
    }
}
impl Into<doomsat_state> for DoomsatState {
    fn into(self) -> doomsat_state {
        let sectors_length = self.sectors.len() as c_int;
        let sectors = Box::into_raw(self.sectors.into_boxed_slice())
            .cast::<doomsat_sector>()
            .cast_const();
        let sides_length = self.sides.len() as c_int;
        let sides = Box::into_raw(self.sides.into_boxed_slice())
            .cast::<doomsat_side>()
            .cast_const();
        let lines_length = self.lines.len() as c_int;
        let lines = Box::into_raw(self.lines.into_boxed_slice())
            .cast::<doomsat_line>()
            .cast_const();
        let mobj_length = self.mobjs.len() as c_int;
        let mobjs = Box::into_raw(self.mobjs.into_boxed_slice())
            .cast::<doomsat_mobj>()
            .cast_const();

        doomsat_state {
            gamestate: self.gamestate,
            gametic: self.gametic,
            leveltime: self.leveltime,
            paused: self.paused,
            automapactive: self.automapactive,
            menuactive: self.menuactive,
            viewx: self.viewx,
            viewy: self.viewy,
            viewz: self.viewz,
            viewangle: self.viewangle,
            player_viewz: self.player_viewz,
            player_extralight: self.player_extralight,
            player_fixedcolormap: self.player_fixedcolormap,
            player_health: self.player_health,
            player_armorpoints: self.player_armorpoints,
            player_ammo: self.player_ammo,
            player_maxammo: self.player_maxammo,
            player_readyweapon: self.player_readyweapon,
            player_weaponowned: self.player_weaponowned,
            player_cards: self.player_cards,
            player_frags: self.player_frags,
            player_powers: self.player_powers,
            player_message: self.player_message,
            player_psprites: self.player_psprites,
            sectors_length,
            sectors,
            sides_length,
            sides,
            lines_length,
            lines,
            mobj_length,
            mobjs,
        }
    }
}
pub use bindings::doomsat_state;

use crate::doom::bindings::{
    boolean, doomsat_line, doomsat_mobj, doomsat_psprite, doomsat_sector, doomsat_side,
};
