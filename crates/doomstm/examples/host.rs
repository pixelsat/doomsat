use doomstm::{Doom, DoomCallbacks, allocator::memory_stats};
use std::{
    cell::UnsafeCell,
    io::{Read as _, Write as _},
    os::unix::net::{UnixListener, UnixStream},
    sync::mpsc::{self, Receiver},
    thread,
    time::Instant,
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

struct DoomStd {
    initial: Instant,
    key_events: Receiver<(bool, u8)>,
    stdout: std::io::Stdout,
    stderr: std::io::Stderr,
}
impl DoomCallbacks for DoomStd {
    fn log(&mut self, string: &str) {
        println!("[doomstm] {}", string);
    }

    fn putc_stdout(&mut self, ch: u8) {
        self.stdout
            .write_all(&[ch])
            .expect("failed to write stdout");
    }

    fn putc_stderr(&mut self, ch: u8) {
        self.stderr
            .write_all(&[ch])
            .expect("failed to write stderr");
    }

    fn flush_stdout(&mut self) {
        self.stdout.flush().expect("failed to flush stdout");
    }

    fn flush_stderr(&mut self) {
        self.stderr.flush().expect("failed to flush stderr");
    }

    fn sleep(&mut self, ms: u32) {
        std::thread::sleep(std::time::Duration::from_millis(ms as u64));
    }

    fn get_elapsed(&mut self) -> core::time::Duration {
        self.initial.elapsed()
    }

    fn get_key_event(&mut self) -> Option<(bool, u8)> {
        self.key_events.try_recv().ok()
    }
}

fn read_input(mut socket: UnixStream, sender: mpsc::Sender<(bool, u8)>) {
    loop {
        let mut event = [0u8; 2];
        if socket.read_exact(&mut event).is_err() {
            return;
        }
        if sender.send((event[0] != 0, event[1])).is_err() {
            return;
        }
    }
}

const SOCKET_PATH: &str = "/tmp/doomsat.sock";
fn main() {
    let _ = std::fs::remove_file(SOCKET_PATH);
    let listener = UnixListener::bind(SOCKET_PATH).expect("failed to bind Doom socket");
    let (mut socket, _) = listener.accept().expect("failed to accept Doom client");

    let input_socket = socket
        .try_clone()
        .expect("failed to clone Doom client socket");
    let (sender, key_events) = mpsc::channel();
    thread::spawn(move || read_input(input_socket, sender));

    let mut callbacks = DoomStd {
        initial: Instant::now(),
        key_events,
        stdout: std::io::stdout(),
        stderr: std::io::stderr(),
    };
    let wad = include_bytes!("../../../wad/doom1.wad");
    let mut doom = Doom::create(&mut callbacks, wad, DTCM_HEAP.access(), SRAM_HEAP.access());

    let mut max_usage = 0;
    loop {
        doom.tick();
        let usage = unsafe { memory_stats() };
        if usage > max_usage {
            max_usage = max_usage.max(usage);
            println!("max memory usage so far: {} bytes", max_usage);
        }
        let state = doom.get_state();
        state
            .with_serialized(|s| {
                let len = s.len() as u32;
                socket.write_all(&len.to_le_bytes())?;
                socket.write_all(s)
            })
            .expect("failed to serialize Doom state")
            .expect("failed to send Doom state");
    }
}
