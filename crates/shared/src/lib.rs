#![no_std]

#[cfg(all(feature = "stm", feature = "client"))]
compile_error!("features `stm` and `client` cannot be enabled together");

#[allow(nonstandard_style, reason = "bindgen")]
pub mod doom_sys {
    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}

#[cfg(feature = "serde")]
pub mod doom_serde {
    use core::mem::{self, MaybeUninit};

    use rkyv::{
        Archive, Deserialize, Serialize,
        ser::{allocator::SubAllocator, writer::Buffer},
        util::Align,
        with::InlineAsBox,
    };

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

    #[derive(Archive, Serialize, Deserialize)]
    #[repr(C)]
    pub struct DoomsatMpoint {
        x: i32,
        y: i32,
    }
    impl From<doomsat_mpoint> for DoomsatMpoint {
        fn from(value: doomsat_mpoint) -> Self {
            DoomsatMpoint {
                x: value.x,
                y: value.y,
            }
        }
    }
    impl Into<doomsat_mpoint> for ArchivedDoomsatMpoint {
        fn into(self) -> doomsat_mpoint {
            doomsat_mpoint {
                x: self.x.into(),
                y: self.y.into(),
            }
        }
    }

    const _: () = {
        ["mobj"][mem::size_of::<doomsat_mobj>() - mem::size_of::<DoomsatMobj>()];
        ["sector"][mem::size_of::<doomsat_sector>() - mem::size_of::<DoomsatSector>()];
        ["side"][mem::size_of::<doomsat_side>() - mem::size_of::<DoomsatSide>()];
        ["line"][mem::size_of::<doomsat_line>() - mem::size_of::<DoomsatLine>()];
        ["psprite"][mem::size_of::<doomsat_psprite>() - mem::size_of::<DoomsatPsprite>()];
        ["mpoint"][mem::size_of::<doomsat_mpoint>() - mem::size_of::<DoomsatMpoint>()];
    };

    #[derive(Archive, Serialize, Deserialize)]
    pub struct DoomsatState {
        pub gameepisode: i32,
        pub gamemap: i32,
        pub gameskill: i32,
        pub gamestate: i32,
        pub gametic: i32,
        pub leveltime: i32,
        pub paused: boolean,
        pub st_palette: i32,

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
        pub player_psprites: [DoomsatPsprite; 2],

        pub setting_screenblocks: i32,
        pub setting_detail_level: i32,
        pub setting_usegamma: i32,
        pub setting_show_messages: i32,
        pub setting_mouse_sensitivity: i32,
        pub setting_sfx_volume: i32,
        pub setting_music_volume: i32,

        pub hud_st_faceindex: i32,
        pub hud_message_on: i32,
        pub hud_message_line_length: i32,
        pub text: [core::ffi::c_char; 80],

        pub automapactive: boolean,
        pub automap_m_x: i32,
        pub automap_m_y: i32,
        pub automap_m_w: i32,
        pub automap_m_h: i32,
        pub automap_scale_mtof: i32,
        pub automap_grid: i32,
        pub automap_cheating: i32,
        pub automap_lightlev: i32,
        pub automap_markpoints: [DoomsatMpoint; 10],

        pub menuactive: boolean,
        pub menuid: u16,
        pub menu_item_on: i16,
        pub menu_which_skull: i16,
        pub menu_message_to_print: i32,
        pub menu_dialog_length: i32,
        pub menu_dialog: [core::ffi::c_char; 160],

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
                if len <= 0 || ptr.is_null() {
                    &[]
                } else {
                    unsafe { core::slice::from_raw_parts(ptr.cast::<T>(), len as usize) }
                }
            }
            DoomsatState {
                gameepisode: value.gameepisode,
                gamemap: value.gamemap,
                gameskill: value.gameskill,
                gamestate: value.gamestate,
                gametic: value.gametic,
                leveltime: value.leveltime,
                paused: value.paused,
                st_palette: value.st_palette,
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
                player_psprites: value.player_psprites.map(Into::into),
                setting_screenblocks: value.setting_screenblocks,
                setting_detail_level: value.setting_detailLevel,
                setting_usegamma: value.setting_usegamma,
                setting_show_messages: value.setting_showMessages,
                setting_mouse_sensitivity: value.setting_mouseSensitivity,
                setting_sfx_volume: value.setting_sfxVolume,
                setting_music_volume: value.setting_musicVolume,
                hud_st_faceindex: value.hud_st_faceindex,
                hud_message_on: value.hud_message_on,
                hud_message_line_length: value.hud_message_line_length,
                text: value.text,
                automapactive: value.automapactive,
                automap_m_x: value.automap_m_x,
                automap_m_y: value.automap_m_y,
                automap_m_w: value.automap_m_w,
                automap_m_h: value.automap_m_h,
                automap_scale_mtof: value.automap_scale_mtof,
                automap_grid: value.automap_grid,
                automap_cheating: value.automap_cheating,
                automap_lightlev: value.automap_lightlev,
                automap_markpoints: value.automap_markpoints.map(Into::into),
                menuactive: value.menuactive,
                menuid: value.menuid,
                menu_item_on: value.menu_itemOn,
                menu_which_skull: value.menu_whichSkull,
                menu_message_to_print: value.menu_messageToPrint,
                menu_dialog_length: value.menu_dialog_length,
                menu_dialog: value.menu_dialog,
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
                gameepisode: self.gameepisode.into(),
                gamemap: self.gamemap.into(),
                gameskill: self.gameskill.into(),
                gamestate: self.gamestate.into(),
                gametic: self.gametic.into(),
                leveltime: self.leveltime.into(),
                paused: self.paused.into(),
                st_palette: self.st_palette.into(),
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
                player_psprites: core::array::from_fn(|i| doomsat_psprite {
                    sx: self.player_psprites[i].sx.into(),
                    sy: self.player_psprites[i].sy.into(),
                    state: self.player_psprites[i].state.into(),
                }),
                setting_screenblocks: self.setting_screenblocks.into(),
                setting_detailLevel: self.setting_detail_level.into(),
                setting_usegamma: self.setting_usegamma.into(),
                setting_showMessages: self.setting_show_messages.into(),
                setting_mouseSensitivity: self.setting_mouse_sensitivity.into(),
                setting_sfxVolume: self.setting_sfx_volume.into(),
                setting_musicVolume: self.setting_music_volume.into(),
                hud_st_faceindex: self.hud_st_faceindex.into(),
                hud_message_on: self.hud_message_on.into(),
                hud_message_line_length: self.hud_message_line_length.into(),
                text: core::array::from_fn(|i| self.text[i].into()),
                automapactive: self.automapactive.into(),
                automap_m_x: self.automap_m_x.into(),
                automap_m_y: self.automap_m_y.into(),
                automap_m_w: self.automap_m_w.into(),
                automap_m_h: self.automap_m_h.into(),
                automap_scale_mtof: self.automap_scale_mtof.into(),
                automap_grid: self.automap_grid.into(),
                automap_cheating: self.automap_cheating.into(),
                automap_lightlev: self.automap_lightlev.into(),
                automap_markpoints: core::array::from_fn(|i| doomsat_mpoint {
                    x: self.automap_markpoints[i].x.into(),
                    y: self.automap_markpoints[i].y.into(),
                }),
                menuactive: self.menuactive.into(),
                menuid: self.menuid.into(),
                menu_itemOn: self.menu_item_on.into(),
                menu_whichSkull: self.menu_which_skull.into(),
                menu_messageToPrint: self.menu_message_to_print.into(),
                menu_dialog_length: self.menu_dialog_length.into(),
                menu_dialog: core::array::from_fn(|i| self.menu_dialog[i].into()),
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
        pub fn with_serialized<T, F>(self, f: F) -> Option<T>
        where
            F: FnOnce(&[u8]) -> T,
        {
            let mut out = Align([0u8; SERIALIZED_STATE_SIZE]);
            let mut scratch = [MaybeUninit::<u8>::uninit(); 0];
            let bytes = rkyv::api::low::to_bytes_in_with_alloc::<_, _, rkyv::rancor::Failure>(
                &self,
                Buffer::from(&mut *out),
                SubAllocator::new(&mut scratch),
            )
            .ok()?;
            Some(f(&bytes))
        }
        pub fn deserialize(bytes: &[u8]) -> Option<&ArchivedDoomsatState> {
            rkyv::api::low::access::<ArchivedDoomsatState, rkyv::rancor::Failure>(bytes).ok()
        }
    }
}
