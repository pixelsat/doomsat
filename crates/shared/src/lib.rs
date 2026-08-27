#![no_std]

#[cfg(all(feature = "stm", feature = "client"))]
compile_error!("features `stm` and `client` cannot be enabled together");

#[allow(nonstandard_style, reason = "bindgen")]
pub mod doom_sys {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[cfg(feature = "serde")]
pub mod doom_serde {
    use core::{mem::{self, MaybeUninit}};

    use rkyv::{Archive, Deserialize, Serialize, ser::{allocator::SubAllocator, writer::Buffer}, util::Align, with::InlineAsBox};

use crate::doom_sys::*;

    #[derive(Archive, Serialize, Deserialize)]
    pub struct DoomsatMobj {
        x: i32,
        y: i32,
        z: i32,
        angle: u32,
        sprite: i32,
        frame: i32,
        flags: i32,
    }
    impl From<doomsat_mobj> for DoomsatMobj {
        fn from(value: doomsat_mobj) -> Self {
            DoomsatMobj {
                x: value.x,
                y: value.y,
                z: value.z,
                angle: value.angle,
                sprite: value.sprite,
                frame: value.frame,
                flags: value.flags,
            }
        }
    }
    impl Into<doomsat_mobj> for ArchivedDoomsatMobj {
        fn into(self) -> doomsat_mobj {
            doomsat_mobj {
                x: self.x.into(),
                y: self.y.into(),
                z: self.z.into(),
                angle: self.angle.into(),
                sprite: self.sprite.into(),
                frame: self.frame.into(),
                flags: self.flags.into(),
            }
        }
    }

    #[derive(Archive, Serialize, Deserialize)]
    #[repr(C)]
    pub struct DoomsatSector {
        floorheight: i32,
        ceilingheight: i32,
        floorpic: i16,
        ceilingpic: i16,
        lightlevel: i16,
    }
    impl From<doomsat_sector> for DoomsatSector {
        fn from(value: doomsat_sector) -> Self {
            DoomsatSector {
                floorheight: value.floorheight,
                ceilingheight: value.ceilingheight,
                floorpic: value.floorpic,
                ceilingpic: value.ceilingpic,
                lightlevel: value.lightlevel,
            }
        }
    }
    impl Into<doomsat_sector> for ArchivedDoomsatSector {
        fn into(self) -> doomsat_sector {
            doomsat_sector {
                floorheight: self.floorheight.into(),
                ceilingheight: self.ceilingheight.into(),
                floorpic: self.floorpic.into(),
                ceilingpic: self.ceilingpic.into(),
                lightlevel: self.lightlevel.into(),
            }
        }
    }

    #[derive(Archive, Serialize, Deserialize)]
    #[repr(C)]
    pub struct DoomsatSide {
        textureoffset: i32,
        rowoffset: i32,
        toptexture: i16,
        bottomtexture: i16,
        midtexture: i16,
    }
    impl From<doomsat_side> for DoomsatSide {
        fn from(value: doomsat_side) -> Self {
            DoomsatSide {
                textureoffset: value.textureoffset,
                rowoffset: value.rowoffset,
                toptexture: value.toptexture,
                bottomtexture: value.bottomtexture,
                midtexture: value.midtexture,
            }
        }
    }
    impl Into<doomsat_side> for ArchivedDoomsatSide {
        fn into(self) -> doomsat_side {
            doomsat_side {
                textureoffset: self.textureoffset.into(),
                rowoffset: self.rowoffset.into(),
                toptexture: self.toptexture.into(),
                bottomtexture: self.bottomtexture.into(),
                midtexture: self.midtexture.into(),
            }
        }
    }

    #[derive(Archive, Serialize, Deserialize)]
    #[repr(C)]
    pub struct DoomsatLine {
        flags: i16,
    }
    impl From<doomsat_line> for DoomsatLine {
        fn from(value: doomsat_line) -> Self {
            DoomsatLine { flags: value.flags }
        }
    }
    impl Into<doomsat_line> for DoomsatLine {
        fn into(self) -> doomsat_line {
            doomsat_line { flags: self.flags }
        }
    }

    #[derive(Archive, Serialize, Deserialize)]
    #[repr(C)]
    pub struct DoomsatPsprite {
        sx: i32,
        sy: i32,
        state: i32,
    }

    impl From<doomsat_psprite> for DoomsatPsprite {
        fn from(value: doomsat_psprite) -> Self {
            DoomsatPsprite {
                sx: value.sx,
                sy: value.sy,
                state: value.state,
            }
        }
    }
    impl Into<doomsat_psprite> for ArchivedDoomsatPsprite {
        fn into(self) -> doomsat_psprite {
            doomsat_psprite {
                sx: self.sx.into(),
                sy: self.sy.into(),
                state: self.state.into(),
            }
        }
    }

    const _: () = {
        ["mobj"][mem::size_of::<doomsat_mobj>() - mem::size_of::<DoomsatMobj>()];
        ["sector"][mem::size_of::<doomsat_sector>() - mem::size_of::<DoomsatSector>()];
        ["side"][mem::size_of::<doomsat_side>() - mem::size_of::<DoomsatSide>()];
        ["line"][mem::size_of::<doomsat_line>() - mem::size_of::<DoomsatLine>()];
        ["psprite"][mem::size_of::<doomsat_psprite>() - mem::size_of::<DoomsatPsprite>()];
    };

    #[derive(Archive, Serialize, Deserialize)]
    pub struct DoomsatState {
        pub gamestate: i32,
        pub gametic: i32,
        pub leveltime: i32,
        pub paused: boolean,
        pub automapactive: boolean,
        pub menuactive: boolean,
        pub viewx: i32,
        pub viewy: i32,
        pub viewz: i32,
        pub viewangle: u32,
        pub player_viewz: i32,
        pub player_extralight: i32,
        pub player_fixedcolormap: i32,
        pub player_health: i32,
        pub player_armorpoints: i32,
        pub player_ammo: [i32; 4],
        pub player_maxammo: [i32; 4],
        pub player_readyweapon: i32,
        pub player_weaponowned: [boolean; 9],
        pub player_cards: [boolean; 6],
        pub player_frags: [i32; 4],
        pub player_powers: [i32; 6],
        pub player_message: u16,
        pub player_psprites: [DoomsatPsprite; 2],
        #[rkyv(with = InlineAsBox)]
        pub sectors: &'static [DoomsatSector],
        #[rkyv(with = InlineAsBox)]
        pub sides: &'static [DoomsatSide],
        #[rkyv(with = InlineAsBox)]
        pub lines: &'static [DoomsatLine],
        #[rkyv(with = InlineAsBox)]
        pub mobjs: &'static [DoomsatMobj],
    }
    impl From<doomsat_state> for DoomsatState {
        fn from(value: doomsat_state) -> Self {
            fn get_slice<T, O>(ptr: *const O, len: i32) -> &'static [T] {
                unsafe { core::slice::from_raw_parts(ptr as *const T, len as usize) }
            }
            DoomsatState {
                gamestate: value.gamestate,
                gametic: value.gametic,
                leveltime: value.leveltime,
                paused: value.paused,
                automapactive: value.automapactive,
                menuactive: value.menuactive,
                viewx: value.viewx,
                viewy: value.viewy,
                viewz: value.viewz,
                viewangle: value.viewangle,
                player_viewz: value.player_viewz,
                player_extralight: value.player_extralight,
                player_fixedcolormap: value.player_fixedcolormap,
                player_health: value.player_health,
                player_armorpoints: value.player_armorpoints,
                player_ammo: value.player_ammo,
                player_maxammo: value.player_maxammo,
                player_readyweapon: value.player_readyweapon,
                player_weaponowned: value.player_weaponowned,
                player_cards: value.player_cards,
                player_frags: value.player_frags,
                player_powers: value.player_powers,
                player_message: value.player_message,
                player_psprites: value.player_psprites.map(|psp| psp.into()),
                sectors: get_slice(value.sectors, value.sectors_length),
                sides: get_slice(value.sides, value.sides_length),
                lines: get_slice(value.lines, value.lines_length),
                mobjs: get_slice(value.mobjs, value.mobj_length),
            }
        }
    }
    impl Into<doomsat_state> for &ArchivedDoomsatState {
        fn into(self) -> doomsat_state {
            doomsat_state {
                gamestate: self.gamestate.into(),
                gametic: self.gametic.into(),
                leveltime: self.leveltime.into(),
                paused: self.paused.into(),
                automapactive: self.automapactive.into(),
                menuactive: self.menuactive.into(),
                viewx: self.viewx.into(),
                viewy: self.viewy.into(),
                viewz: self.viewz.into(),
                viewangle: self.viewangle.into(),
                player_viewz: self.player_viewz.into(),
                player_extralight: self.player_extralight.into(),
                player_fixedcolormap: self.player_fixedcolormap.into(),
                player_health: self.player_health.into(),
                player_armorpoints: self.player_armorpoints.into(),
                player_ammo: core::array::from_fn(|i| self.player_ammo[i].into()),
                player_maxammo: core::array::from_fn(|i| self.player_maxammo[i].into()),
                player_readyweapon: self.player_readyweapon.into(),
                player_weaponowned: core::array::from_fn(|i| self.player_weaponowned[i].into()),
                player_cards: core::array::from_fn(|i| self.player_cards[i].into()),
                player_frags: core::array::from_fn(|i| self.player_frags[i].into()),
                player_powers: core::array::from_fn(|i| self.player_powers[i].into()),
                player_message: self.player_message.into(),
                player_psprites: core::array::from_fn(|i| unsafe {
                    core::ptr::read(&self.player_psprites[i]).into()
                }),
                sectors_length: self.sectors.len() as i32,
                sectors: self.sectors.as_ptr() as *const _,
                sides_length: self.sides.len() as i32,
                sides: self.sides.as_ptr() as *const _,
                lines_length: self.lines.len() as i32,
                lines: self.lines.as_ptr() as *const _,
                mobj_length: self.mobjs.len() as i32,
                mobjs: self.mobjs.as_ptr() as *const _,
            }
        }
    }

    pub const SERIALIZED_STATE_SIZE: usize = 64 * 1024;

    impl DoomsatState {
        pub fn with_serialized<T, F>(self, f: F) -> Option<T> where F: FnOnce(&[u8]) -> T {
            let mut out = Align([0u8; SERIALIZED_STATE_SIZE]);
            let mut scratch = [MaybeUninit::<u8>::uninit(); 0];
            let bytes = rkyv::api::low::to_bytes_in_with_alloc::<_, _, rkyv::rancor::Failure>(
                &self,
                Buffer::from(&mut *out),
                SubAllocator::new(&mut scratch),
            ).ok()?;
            Some(f(&bytes))
        }
        pub fn deserialize(bytes: &[u8]) -> Option<&ArchivedDoomsatState> {
            rkyv::api::low::access::<ArchivedDoomsatState, rkyv::rancor::Failure>(bytes).ok()
        }
    }
}
