use std::ffi::{c_char, c_int, CString};

mod doomclient_unsafe {
    use std::{
        ffi::{CStr, c_char, c_int}, slice, thread, time::Duration,
    };
    use std::{sync::OnceLock, time::Instant};

    static INIT_TIME: OnceLock<Instant> = OnceLock::new();
    unsafe extern "C" {
        pub fn doomgeneric_Create(argc: c_int, argv: *mut *mut c_char);
        pub fn doomgeneric_Tick();

        static mut DG_ScreenBuffer: *mut u32;
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_Init() {
        INIT_TIME.set(Instant::now()).ok();
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_DrawFrame() {
        let pixels = unsafe { slice::from_raw_parts(DG_ScreenBuffer, 640 * 400) };

        println!("draw");
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_SleepMs(ms: u32) {
        thread::sleep(Duration::from_millis(ms.into()));
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_GetTicksMs() -> u32 {
        INIT_TIME.get().map(|t| t.elapsed().as_millis() as u32).unwrap_or(0)
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_GetKey(pressed: *mut c_int, key: *mut u8) -> c_int {
        // Pop an event from your Rust input queue.
        // Write 1 or 0 to `pressed`, write the Doom key code to `key`,
        // and return 1. Return 0 when the queue is empty.
        0
    }

    #[unsafe(no_mangle)]
    pub extern "C" fn DG_SetWindowTitle(title: *const c_char) {
        let title = unsafe { CStr::from_ptr(title) };
        println!("set title: {:?}", title);
    }
}

pub fn create(args: &[String]) {
    let argv = args
        .iter()
        .map(|arg| CString::new(arg.as_str()).expect("arguments cannot contain NUL bytes"))
        .collect::<Vec<_>>();
    let mut argv_ptrs = argv
        .iter()
        .map(|arg| arg.as_ptr() as *mut c_char)
        .collect::<Vec<_>>();

    unsafe { doomclient_unsafe::doomgeneric_Create(argv_ptrs.len() as c_int, argv_ptrs.as_mut_ptr()) };

    std::mem::forget(argv);
    std::mem::forget(argv_ptrs);
}

pub fn tick() {
    unsafe { doomclient_unsafe::doomgeneric_Tick() };
}
