mod doomclient;

use std::{
    path::PathBuf
};

fn main() {
    let wad = PathBuf::from("../wad/doom1.wad");
    doomclient::create(&[
        "doomclient".to_owned(),
        "-iwad".to_owned(),
        wad.to_string_lossy().into_owned(),
        "-skill".to_owned(),
        "5".to_owned(),
        "-warp".to_owned(),
        "1".to_owned(),
        "9".to_owned(),
    ]);

    loop {
        doomclient::tick();
    }
}