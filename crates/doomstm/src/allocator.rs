// We use UnsafeCell gratuitously to avoid any overhead, since allocations are obviously
// very very common.

use core::{
    cell::UnsafeCell,
    ffi::{c_char, c_int, c_void},
    mem::{self, MaybeUninit},
    ptr::{NonNull, null_mut},
};

use crate::sys::CALLBACKS;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Tag {
    Static,
    Free,
    Level,
    LevelSpecial,
    Cache,
    Sentinel,
}
impl Tag {
    fn from_c(n: u8) -> Self {
        match n {
            1 | 2 | 3 => Tag::Static,
            4 => Tag::Free,
            5 => Tag::Level,
            6 => Tag::LevelSpecial,
            8 => Tag::Cache,
            255 => Tag::Sentinel,
            _ => unreachable!(),
        }
    }
    fn to_c(&self) -> u8 {
        match self {
            Tag::Static => 1,
            Tag::Free => 4,
            Tag::Level => 5,
            Tag::LevelSpecial => 6,
            Tag::Cache => 8,
            Tag::Sentinel => 255,
        }
    }
}

const ID: u32 = 0x1D4A11;

struct Block {
    size: usize,
    owner: Option<NonNull<*mut u8>>,
    tag: u8,
    id: u32,
    prev: *mut Block,
    next: *mut Block,
}
impl Block {
    fn merge(this: *mut Block, other: *mut Block) {
        unsafe {
            debug_assert!(Tag::from_c((*this).tag) == Tag::Free);
            debug_assert!(Tag::from_c((*other).tag) == Tag::Free);

            (*this).next = (*other).next;
            (*(*other).next).prev = this;
            (*this).size += (*other).size;
        }
    }
    fn sentinel() -> Self {
        Block {
            size: 0,
            owner: None,
            tag: 255,
            id: 255,
            prev: null_mut(),
            next: null_mut(),
        }
    }
}

enum Bank {
    Dtcm,
    Sram,
}
impl Bank {
    fn access(&self) -> &mut Zone {
        match self {
            Bank::Dtcm => unsafe { (&mut *DTCM_ZONE.0.get()).assume_init_mut() },
            Bank::Sram => unsafe { (&mut *SRAM_ZONE.0.get()).assume_init_mut() },
        }
    }
    fn set(&self, zone: Zone) {
        *self.access() = zone;
    }
}

struct ZoneManager(UnsafeCell<MaybeUninit<Zone>>);
unsafe impl Send for ZoneManager {}
unsafe impl Sync for ZoneManager {}

static DTCM_ZONE: ZoneManager = ZoneManager(UnsafeCell::new(MaybeUninit::uninit()));
static SRAM_ZONE: ZoneManager = ZoneManager(UnsafeCell::new(MaybeUninit::uninit()));

struct Zone {
    size: usize,
    start: *mut u8,
    sentinel: Block,
}
impl Zone {
    unsafe fn new(storage: &'static mut [u8], size: usize) -> Zone {
        let start = storage.as_mut_ptr();
        debug_assert!((start as usize) % align_of::<Block>() == 0);
        Zone {
            start,
            size,
            sentinel: Block::sentinel(),
        }
    }
    // not in new bc reutrning Zone moves it etc. etc.
    unsafe fn init(&mut self) {
        let first = self.start as *mut Block;

        unsafe {
            (*first).size = self.size;
            (*first).owner = None;
            (*first).tag = Tag::Free.to_c();
            (*first).id = 0;
            (*first).prev = &mut self.sentinel;
            (*first).next = &mut self.sentinel;
        }

        self.sentinel.next = first;
        self.sentinel.prev = first;
    }
    fn allocation_size(requested: usize) -> usize {
        let raw = (requested + size_of::<Block>()).next_multiple_of(align_of::<Block>());

        if raw < 1024 {
            raw
        } else {
            raw.next_multiple_of(512)
        }
    }
    fn contains_payload(&self, ptr: *mut u8) -> bool {
        let ptr = ptr as usize;
        let start = self.start as usize;
        (ptr) >= (start + size_of::<Block>()) && (ptr) < (start + self.size)
    }
    fn preferred_banks(requested: usize, tag: Tag) -> [Bank; 2] {
        let small_level_allocation =
            matches!(tag, Tag::Level | Tag::LevelSpecial) && requested < 4096;

        if small_level_allocation {
            [Bank::Dtcm, Bank::Sram]
        } else {
            [Bank::Sram, Bank::Dtcm]
        }
    }
    // inclusive
    unsafe fn free_tag(&mut self, low: u8, high: u8) {
        unsafe {
            let mut cursor: *mut Block = self.sentinel.next;
            loop {
                let next = (*cursor).next;

                let tag = (*cursor).tag;
                if tag == Tag::Sentinel.to_c() {
                    break;
                }
                if low <= tag && tag <= high && tag != Tag::Free.to_c() {
                    self.free((cursor as *mut u8).add(size_of::<Block>()));
                }

                cursor = next;
            }
        }
    }
    unsafe fn free(&mut self, pointer: *mut u8) {
        unsafe {
            if !self.contains_payload(pointer) {
                return;
            }
            let mut block = pointer.sub(size_of::<Block>()) as *mut Block;
            if (*block).id != ID {
                return;
            }
            if let Some(owner) = (*block).owner {
                *owner.as_ptr() = null_mut();
            }

            (*block).owner = None;
            (*block).tag = Tag::Free.to_c();
            (*block).id = 0;

            let prev = (*block).prev;
            if (*prev).tag == Tag::Free.to_c() {
                Block::merge(prev, block);
                block = prev;
            }

            let next = (*block).next;
            if (*next).tag == Tag::Free.to_c() {
                Block::merge(block, next);
            }
        }
    }
    unsafe fn try_allocate(
        &mut self,
        size: usize,
        tag: u8,
        owner: Option<NonNull<*mut u8>>,
    ) -> Option<NonNull<u8>> {
        unsafe {
            let size = Zone::allocation_size(size);

            let mut cursor: *mut Block = &mut self.sentinel;

            loop {
                let next = (*cursor).next;

                // at the beginning of this block, cursor is known
                // to not be a viable block to use
                match Tag::from_c((*next).tag) {
                    Tag::Cache => {
                        let ptr = (next as *mut u8).add(mem::size_of::<Block>());
                        self.free(ptr);

                        // The cache block may have merged into cursor, so
                        // reconsider cursor before advancing past it.
                        if (*cursor).tag == Tag::Free.to_c() && (*cursor).size >= size {
                            break;
                        }
                        continue;
                    }
                    Tag::Free => {
                        cursor = next;
                        if (*next).size >= size {
                            break;
                        }
                    }
                    Tag::Sentinel => {
                        // loop is over
                        return None;
                    }
                    _ => {
                        cursor = next;
                    }
                }
            }

            // cursor is a free block that is large enough to fit the request

            // leftover room in the free block
            let extra = (*cursor).size - size;

            // don't create a free block unless there's more than 64 bytes for it
            if extra > 64 {
                let new_free = (cursor as *mut u8).add(size) as *mut Block;

                (*new_free).size = extra;
                (*new_free).owner = None;
                (*new_free).tag = Tag::Free.to_c();
                (*new_free).id = 0;

                (*new_free).prev = cursor;
                (*new_free).next = (*cursor).next;
                (*(*new_free).next).prev = new_free;

                (*cursor).next = new_free;
                (*cursor).size = size;
            }
            // otherwise cursor->size is alr right

            (*cursor).owner = owner;
            (*cursor).tag = tag;
            (*cursor).id = ID;

            let result = NonNull::new_unchecked((cursor as *mut u8).add(size_of::<Block>()));

            if let Some(owner_slot) = owner {
                *owner_slot.as_ptr() = result.as_ptr();
            }

            Some(result)
        }
    }

    unsafe fn block_for(ptr: *mut u8) -> Option<NonNull<Block>> {
        unsafe {
            let block = ptr.sub(size_of::<Block>()) as *mut Block;

            if (*block).id != ID {
                None
            } else {
                Some(NonNull::new_unchecked(block))
            }
        }
    }

    unsafe fn change_tag(&mut self, ptr: *mut u8, new_tag: u8) {
        unsafe {
            if let Some(block) = Self::block_for(ptr) {
                if Tag::from_c(new_tag) == Tag::Cache && (*block.as_ptr()).owner.is_none() {
                    CALLBACKS.with(|c| c.log("ownerless cache block tag change ignored"));
                    return;
                }
                (*block.as_ptr()).tag = new_tag;
            }
        }
    }

    unsafe fn change_owner(&mut self, ptr: *mut u8, new_owner: *mut *mut u8) {
        unsafe {
            if let Some(block) = Self::block_for(ptr) {
                (*block.as_ptr()).owner = NonNull::new(new_owner);
                if !new_owner.is_null() {
                    *new_owner = ptr;
                }
            }
        }
    }

    unsafe fn free_memory(&self) -> usize {
        unsafe {
            let mut total = 0;
            let mut block: *mut Block = self.sentinel.next;

            while (*block).tag != Tag::Sentinel.to_c() {
                let tag = Tag::from_c((*block).tag);
                if tag == Tag::Free || tag == Tag::Cache {
                    total += (*block).size;
                }

                block = (*block).next;
            }

            total
        }
    }
}

pub(crate) fn setup_zones(dtcm: &'static mut [u8], sram: &'static mut [u8]) {
    unsafe {
        Bank::Dtcm.set(Zone::new(dtcm, dtcm.len()));
        Bank::Sram.set(Zone::new(sram, sram.len()));
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_Init() {
    unsafe {
        for zone in [Bank::Dtcm, Bank::Sram] {
            zone.access().init();
        }
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_Malloc(size: c_int, tag: c_int, owner: *mut *mut u8) -> *mut u8 {
    unsafe {
        let owner = NonNull::new(owner);
        let tag = tag.try_into().unwrap_or(0);
        let preferred = Zone::preferred_banks(size as _, Tag::from_c(tag));
        if let Some(ptr) = preferred[0]
            .access()
            .try_allocate(size as usize, tag, owner)
        {
            ptr.as_ptr()
        } else if let Some(ptr) = preferred[1]
            .access()
            .try_allocate(size as usize, tag, owner)
        {
            ptr.as_ptr()
        } else {
            panic!("out of memory")
        }
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_Free(ptr: *mut u8) {
    unsafe {
        for zone in [Bank::Dtcm, Bank::Sram] {
            let zone = zone.access();
            if zone.contains_payload(ptr) {
                zone.free(ptr);
                return;
            }
        }
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_FreeTags(lowtag: c_int, hightag: c_int) {
    unsafe {
        // 254 as sentinel since nothing is 254 (sentinel tag is 255)
        let lowtag = lowtag.try_into().unwrap_or(254);
        let hightag = hightag.try_into().unwrap_or(254);
        for zone in [Bank::Dtcm, Bank::Sram] {
            let zone = zone.access();
            zone.free_tag(lowtag, hightag);
        }
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_DumpHeap(_lowtag: c_int, _hightag: c_int) {
    CALLBACKS.with(|c| c.log("Z_DumpHeap called"));
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_FileDumpHeap(_file: *mut c_void) {
    CALLBACKS.with(|c| c.log("Z_FileDumpHeap called"));
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_CheckHeap() {
    CALLBACKS.with(|c| c.log("Z_CheckHeap called"));
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_ChangeTag2(ptr: *mut u8, tag: c_int, _file: *mut c_char, _line: c_int) {
    unsafe {
        for zone in [Bank::Dtcm, Bank::Sram] {
            let zone = zone.access();
            if zone.contains_payload(ptr) {
                zone.change_tag(ptr, tag.try_into().unwrap_or(0));
                return;
            }
        }
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_ChangeUser(ptr: *mut u8, user: *mut *mut u8) {
    unsafe {
        for zone in [Bank::Dtcm, Bank::Sram] {
            let zone = zone.access();
            if zone.contains_payload(ptr) {
                zone.change_owner(ptr, user);
                return;
            }
        }
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_FreeMemory() -> c_int {
    unsafe {
        (Bank::Dtcm.access().free_memory() + Bank::Sram.access().free_memory())
            .try_into()
            .unwrap_or(c_int::MAX)
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn Z_ZoneSize() -> c_int {
    (Bank::Dtcm.access().size + Bank::Sram.access().size)
        .try_into()
        .unwrap_or(c_int::MAX)
}
