use std::{io::Read, os::unix::net::UnixListener, path::PathBuf, time::{Duration, Instant}};

use doomsat::doom::{Doom, DoomCallbacks, deserialize_state};
use minifb::{Window, WindowOptions};

const MAX_STATE_SIZE: usize = 64 * 1024 * 1024;

struct DoomStub {
    initial_time: Instant,
}
impl DoomCallbacks for DoomStub {
    fn new() -> Self {
        DoomStub {
            initial_time: Instant::now(),
        }
    }
    fn on_draw(&mut self, _buf: &[u32; 320 * 200]) {
        dbg!("on_draw");
    }
    fn get_key_event(&mut self) -> Option<(bool, u8)> {
        dbg!("get_key_event");
        None
    }
    fn sleep(&mut self, ms: u32) {
        std::thread::sleep(std::time::Duration::from_millis(ms.into()));
    }
    fn init(&mut self) {}
    fn set_window_title(&mut self, _title: &str) {}
    fn get_elapsed(&mut self) -> core::time::Duration {
        self.initial_time.elapsed()
    }
}
fn main() {
    let _ = std::fs::remove_file("/tmp/doomsat_state.sock");
    let listener = UnixListener::bind("/tmp/doomsat_state.sock").unwrap();

    let wad = PathBuf::from("../wad/doom1.wad");
    let mut client = DoomStub::new();
    let mut doom = Doom::create(
        &mut client,
        &[
            "doomclient".to_owned(),
            "-iwad".to_owned(),
            wad.to_string_lossy().into_owned(),
            "-skill".to_owned(),
            "5".to_owned(),
            "-warp".to_owned(),
            "1".to_owned(),
            "9".to_owned(),
        ],
    );

    let (mut sock, _) = listener.accept().unwrap();
    let mut buf = Vec::new();

    let mut window = Window::new("Doom", 320, 200, WindowOptions::default())
        .expect("failed to create window");

    loop {
        let mut len = [0; size_of::<u64>()];
        let Ok(_) = sock.read_exact(&mut len) else {
            std::thread::sleep(Duration::from_millis(50));
            continue;
        };
        let len = usize::try_from(u64::from_le_bytes(len)).expect("state is too large");
        assert!(len <= MAX_STATE_SIZE, "state is too large");
        buf.resize(len, 0);
        let Ok(_) = sock.read_exact(&mut buf) else {
            std::thread::sleep(Duration::from_millis(50));
            continue;
        };
        let Ok(state) = deserialize_state(&buf) else {
            std::thread::sleep(Duration::from_millis(50));
            continue;
        };
        doom.draw_state(state.clone());
        doom.with_screen(|s| {
            let _ = window.update_with_buffer(s, 320, 200);
        });
    }
}
