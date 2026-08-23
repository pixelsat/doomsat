mod doomclient;

use std::{
    ffi::{CStr, c_char}, fs, path::PathBuf, slice, thread, time::{Duration, Instant},
};

fn main() {
    let wad = PathBuf::from("../wad/doom1.wad");
    doomclient::create(&[
        "doomclient".to_owned(),
        "-iwad".to_owned(),
        wad.to_string_lossy().into_owned(),
    ]);

    loop {
        doomclient::tick();
    }
}