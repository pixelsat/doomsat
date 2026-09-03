//! Deterministic keyboard fuzzer. Runs the game with a virtual clock (only
//!
//! `sleep` advances time) and a fixed key script, then logs one JSON line of
//! game state per frame. Run it on two builds and diff the output files.

use doomstm::{Doom, DoomCallbacks, allocator::memory_stats};
use std::{
    cell::UnsafeCell,
    collections::VecDeque,
    fs::File,
    io::{BufWriter, Write as _},
    time::Duration,
};

#[repr(transparent)]
struct SimpleZone<const N: usize>(UnsafeCell<[u8; N]>);
unsafe impl<const N: usize> Send for SimpleZone<N> {}
unsafe impl<const N: usize> Sync for SimpleZone<N> {}
impl<const N: usize> SimpleZone<N> {
    fn access(&self) -> &'static mut [u8; N] {
        unsafe { &mut *self.0.get() }
    }
}

static DTCM_HEAP: SimpleZone<{ 1024 * 2048 }> = SimpleZone(UnsafeCell::new([0u8; 1024 * 2048]));
static SRAM_HEAP: SimpleZone<{ 1024 * 2048 }> = SimpleZone(UnsafeCell::new([0u8; 1024 * 2048]));

struct KeyQueue(UnsafeCell<VecDeque<(bool, u8)>>);
unsafe impl Sync for KeyQueue {}
static KEYS: KeyQueue = KeyQueue(UnsafeCell::new(VecDeque::new()));

struct FuzzCallbacks {
    now_ms: u64,
}
impl DoomCallbacks for FuzzCallbacks {
    fn log(&mut self, string: &str) {
        eprintln!("[doomstm] {}", string);
    }
    fn putc_stdout(&mut self, _ch: u8) {}
    fn putc_stderr(&mut self, ch: u8) {
        eprint!("{}", ch as char);
    }
    fn flush_stdout(&mut self) {}
    fn flush_stderr(&mut self) {}
    fn sleep(&mut self, ms: u32) {
        self.now_ms += ms as u64;
    }
    fn get_elapsed(&mut self) -> Duration {
        Duration::from_millis(self.now_ms)
    }
    fn get_key_event(&mut self) -> Option<(bool, u8)> {
        unsafe { (*KEYS.0.get()).pop_front() }
    }
}

const KEY_RIGHT: u8 = 0xae;
const KEY_LEFT: u8 = 0xac;
const KEY_UP: u8 = 0xad;
const KEY_DOWN: u8 = 0xaf;
const KEY_USE: u8 = 0xa2;
const KEY_FIRE: u8 = 0xa3;
const KEY_ESCAPE: u8 = 27;
const KEY_ENTER: u8 = 13;
const KEY_TAB: u8 = 9;

/// (frame, pressed, key), sorted by frame.
fn script() -> Vec<(u32, bool, u8)> {
    let mut v: Vec<(u32, bool, u8)> = Vec::new();
    let mut key = |t: u32, k: u8, dur: u32| {
        v.push((t, true, k));
        v.push((t + dur, false, k));
    };

    key(40, KEY_TAB, 3); // automap on
    key(60, KEY_TAB, 3); // automap off
    key(80, KEY_UP, 60); // walk forward
    key(150, KEY_RIGHT, 20); // turn
    key(175, KEY_FIRE, 5);
    key(185, KEY_USE, 3);

    // automap tour: pan, zoom, grid, marks, big view
    key(200, KEY_TAB, 3);
    key(210, b'f', 3); // follow off
    key(220, KEY_UP, 20); // pan
    key(245, KEY_LEFT, 20);
    key(270, b'=', 15); // zoom in
    key(290, b'-', 15); // zoom out
    key(310, b'g', 3); // grid
    key(320, b'm', 3); // add mark
    key(330, b'0', 3); // max zoom out
    key(340, b'f', 3); // follow on
    key(350, KEY_TAB, 3);

    key(370, b'2', 3); // switch weapon
    key(380, KEY_FIRE, 30);
    key(420, KEY_ESCAPE, 3); // menu open
    key(430, KEY_DOWN, 3);
    key(440, KEY_ESCAPE, 3); // menu close
    key(460, KEY_UP, 120); // walk into the fight
    key(470, KEY_FIRE, 90);
    key(590, KEY_LEFT, 25);
    key(620, KEY_UP, 80);
    key(630, KEY_USE, 3);
    key(710, KEY_TAB, 3); // automap again mid-fight
    key(750, b'=', 20);
    key(780, KEY_TAB, 3);
    key(800, KEY_FIRE, 120);
    key(810, KEY_RIGHT, 40);
    key(940, KEY_ENTER, 3); // dismiss anything
    key(960, KEY_UP, 150);
    key(970, KEY_USE, 3);
    key(1150, KEY_FIRE, 60);

    v.sort_by_key(|e| e.0);
    v
}

fn hash_bytes(h: &mut u64, bytes: &[u8]) {
    for &b in bytes {
        *h = (*h ^ b as u64).wrapping_mul(0x100000001b3);
    }
}
fn fnv<T>(slice: &[T]) -> u64 {
    let mut h = 0xcbf29ce484222325u64;
    let bytes = unsafe {
        std::slice::from_raw_parts(slice.as_ptr().cast::<u8>(), std::mem::size_of_val(slice))
    };
    hash_bytes(&mut h, bytes);
    h
}

fn main() {
    let frames: u32 = std::env::args()
        .nth(1)
        .and_then(|s| s.parse().ok())
        .unwrap_or(1300);
    let out_path = std::env::args()
        .nth(2)
        .unwrap_or_else(|| "fuzz_states.jsonl".into());

    let mut callbacks = FuzzCallbacks { now_ms: 0 };
    let wad = include_bytes!("../../../wad/doom1.wad");
    let mut doom = Doom::create(&mut callbacks, wad, DTCM_HEAP.access(), SRAM_HEAP.access());

    let script = script();
    let mut script_pos = 0;
    let mut out = BufWriter::new(File::create(&out_path).expect("failed to create output"));

    for frame in 0..frames {
        // Deliver this frame's scripted keys, then run one frame.
        while script_pos < script.len() && script[script_pos].0 <= frame {
            let (_, pressed, key) = script[script_pos];
            unsafe { (*KEYS.0.get()).push_back((pressed, key)) };
            script_pos += 1;
        }
        doom.tick();

        let s = doom.get_state();
        writeln!(
            out,
            concat!(
                "{{\"frame\":{},\"gametic\":{},\"leveltime\":{},\"gamestate\":{},",
                "\"viewx\":{},\"viewy\":{},\"viewz\":{},\"viewangle\":{},",
                "\"player_viewz\":{},\"extralight\":{},\"fixedcolormap\":{},",
                "\"health\":{},\"armor\":{},\"readyweapon\":{},\"ammo\":{:?},",
                "\"st_palette\":{},\"faceindex\":{},",
                "\"automapactive\":{},\"am_m_x\":{},\"am_m_y\":{},\"am_m_w\":{},\"am_m_h\":{},",
                "\"am_scale\":{},\"am_grid\":{},\"am_cheating\":{},\"am_marks\":{},",
                "\"menuactive\":{},\"menuid\":{},\"paused\":{},",
                "\"n_sectors\":{},\"n_sides\":{},\"n_lines\":{},\"n_mobjs\":{},",
                "\"h_sectors\":{},\"h_sides\":{},\"h_lines\":{},\"h_mobjs\":{},\"h_psprites\":{},",
                "\"mem\":{}}}"
            ),
            frame,
            s.gametic,
            s.leveltime,
            s.gamestate,
            s.viewx,
            s.viewy,
            s.viewz,
            s.viewangle,
            s.player_viewz,
            s.player_extralight,
            s.player_fixedcolormap,
            s.player_health,
            s.player_armorpoints,
            s.player_readyweapon,
            s.player_ammo,
            s.st_palette,
            s.hud_st_faceindex,
            s.automapactive,
            s.automap_m_x,
            s.automap_m_y,
            s.automap_m_w,
            s.automap_m_h,
            s.automap_scale_mtof,
            s.automap_grid,
            s.automap_cheating,
            fnv(&s.automap_markpoints),
            s.menuactive,
            s.menuid,
            s.paused,
            s.sectors.len(),
            s.sides.len(),
            s.lines.len(),
            s.mobjs.len(),
            fnv(s.sectors),
            fnv(s.sides),
            fnv(s.lines),
            fnv(s.mobjs),
            fnv(&s.player_psprites),
            unsafe { memory_stats() },
        )
        .expect("failed to write state line");
    }

    out.flush().expect("failed to flush output");
    eprintln!("wrote {} frames to {}", frames, out_path);
}
