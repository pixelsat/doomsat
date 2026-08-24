# doomstm state schema (rough)

What has to cross the wire from doomstm to doomclient for the client to render a frame.
Assumption throughout: doomclient loads the same pruned WAD and runs the same
`P_SetupLevel`, so all static geometry and lump-derived tables are rebuilt locally and
never transmitted. Only mutable state ships. Paths are relative to `doomstm/`.

Reference: `p_saveg.c` is vanilla Doom's own full-state serializer
(`P_ArchivePlayers` @ p_saveg.c:1440, `P_ArchiveWorld` @ :1484, `P_ArchiveThinkers` @
:1592, `P_ArchiveSpecials` @ :1704). It also shows the pointer-flattening tricks
(states stored as index into `states[]`). Our schema is the render-relevant subset.

The render entry points this schema feeds (`D_Display` @ d_main.c:169):
`R_RenderPlayerView` (d_main.c:244), `ST_Drawer` (:222), `HU_Drawer` (:247),
`AM_Drawer` (:217), `M_Drawer` (:297). Plan: keep only the `GS_LEVEL` path, rip out
wipe/intermission/finale/menu drawing.

## 1. View / POV block

| field | declared | written by sim | read by renderer |
|---|---|---|---|
| `viewx, viewy, viewz` | r_main.c:70-72 (externs r_state.h:97-99) | `R_SetupFrame` copies from `player->mo->x/y` + `player->viewz` (r_main.c:823-833) | everywhere in r_bsp.c/r_segs.c/r_things.c (e.g. r_things.c:475) |
| `viewangle` | r_main.c:74 (extern r_state.h:101) | from `player->mo->angle` (r_main.c:830) | r_main.c, r_segs.c |
| `player->viewz` | d_player.h:87 | p_user.c (`P_CalcHeight`) | via R_SetupFrame |
| `player->extralight` | d_player.h:145 | p_pspr.c (muzzle flash) | copied to `extralight` (r_main.c:831), used r_things.c:620, r_segs.c, r_plane.c |
| `player->fixedcolormap` | d_player.h:149 | p_user.c (invuln/light goggles) | r_main.c:840-852, r_things.c:575-578 |

Transmit: 4 fixeds + 1 angle + 2 small ints. Client calls its own `R_SetupFrame` off a
dummy player, or we bypass it and poke the globals directly.

## 2. Things (mobjs)

Source of truth: the thinker list, `thinkercap` @ p_tick.c:40, walked for entries whose
function is `P_MobjThinker`. Struct is `mobj_t` @ p_mobj.h.

Renderer (`R_ProjectSprite`, r_things.c) reads exactly these fields per thing:

| field | declared | read by renderer |
|---|---|---|
| `x, y` | p_mobj.h:207-208 | r_things.c:475-476, 514, 545-546 |
| `z` | p_mobj.h:209 | r_things.c:547-548 |
| `angle` | p_mobj.h:216 | r_things.c:515 (8-rotation select) |
| `sprite` | p_mobj.h:217 | r_things.c:499-509 |
| `frame` | p_mobj.h:218 | r_things.c:505-509, `FF_FULLBRIGHT` @ :580 |
| `flags` | p_mobj.h:248 | r_things.c:543; `MF_SHADOW` fuzz @ :570; also `MF_TRANSLATION` color remap |

That's the whole per-thing payload (~16-20 bytes packed). Everything else in `mobj_t`
(momentum, target, health, state timers, blockmap links) is sim-only.

Client-side wrinkle: sprites are drawn per-subsector by walking `sector->thinglist` /
`thing->snext` (r_things.c:630, list head declared r_defs.h:123). Received mobjs must
be re-linked into sectors — either a stripped `P_SetThingPosition` (p_maputl.c) or
`R_PointInSubsector(x, y)->sector` per mobj.

Identity: things need a stable id for delta encoding. Vanilla has none; simplest is
spawn-order index assigned in doomstm.

## 3. Sectors (mutable fields only)

Struct: `sector_t` @ r_defs.h:79-131. Arrays: `sectors` declared p_setup.c:58,
`numsectors` p_setup.c:57 (externs r_state.h:78-79).

| field | mutated by | read by renderer |
|---|---|---|
| `floorheight` | p_floor.c:64+ (`T_MovePlane`), p_plats.c | r_bsp.c:320-326, r_bsp.c:509-513, r_segs.c |
| `ceilingheight` | p_floor.c:131+, p_ceilng.c, p_doors.c | r_bsp.c:320-326, 518+ |
| `floorpic`, `ceilingpic` | p_spec.c (transfers, rarely) | r_bsp.c:334-335, r_plane.c |
| `lightlevel` | p_lights.c:49-51, 98 (flicker/strobe/glow) | r_bsp.c:336, r_things.c:620, r_plane.c |

Transmit per sector: 2 fixeds + 3 shorts (~12-14B). E1M1 is small enough to snapshot
whole; deltas trivial later. Not transmitted: `soundtraversed`, `soundtarget`,
`validcount`, `thinglist`, `specialdata`, `blockbox` — all sim/renderer-local
bookkeeping (r_defs.h:103-129).

The active movers themselves (`ceiling_t`, `vldoor_t`, `plat_t`, `floormove_t` in
p_spec.h, ticked from p_tick.c) never ship; the height/light snapshot fully captures
their visible effect.

## 4. Sidedefs (rarely change)

Struct: `side_t` @ r_defs.h:140-157. Array `sides` @ p_setup.c:70, `numsides` @ :69.

| field | mutated by | read by renderer |
|---|---|---|
| `textureoffset` | p_spec.c:1131 (scrolling wall special 48) | r_segs.c:632 |
| `rowoffset` | never at runtime (vanilla) | r_segs.c:149, 471, 600-601 |
| `toptexture` / `midtexture` / `bottomtexture` | p_switch.c:223, 235, 247 (switch press) | r_segs.c:456, 570, 589 |

Delta-encode; E1M1 has one scroller and a handful of switches.

## 5. Lines (automap only)

Array `lines` @ p_setup.c:67, `numlines` @ :66. Only mutable render-relevant bit is
`ML_MAPPED` in `flags`, set in p_map.c when a line is crossed, read by `AM_Drawer`
@ am_map.c:1134. Skip entirely if we drop the automap; otherwise a bitset
(numlines bits).

## 6. Player HUD block

Struct `player_t` @ d_player.h. Read every frame by st_stuff.c (via `plyr->`) and
hu_stuff.c.

| field | declared | read by |
|---|---|---|
| `health` | d_player.h:97 | st_stuff.c:671, 700 |
| `armorpoints` | d_player.h:98 | st_stuff.c widgets |
| `ammo[NUMAMMO]`, `maxammo` | d_player.h:115 | st_stuff.c widgets |
| `readyweapon` | d_player.h:109 | st_stuff.c (arms/ammo select) |
| `weaponowned[NUMWEAPONS]` | d_player.h:114 | st_stuff.c:717 |
| `cards[NUMCARDS]` | d_player.h:104 | st_stuff.c keybox |
| `frags` | d_player.h:108 | st_stuff.c (netgame only — skip) |
| `message` | d_player.h:135 | hu_stuff.c:419-423 |

`message` is a pointer to a static string; transmit as short inline string (or an id
into the strings table since all messages come from d_englsh.h).

Also `attackdown`/face state: `ST_updateFaceWidget` in st_stuff.c reads `plyr->damagecount`,
`plyr->attacker` position (for face direction), `plyr->cheats`. Cheapest is to compute
the face on doomstm and ship one byte (`st_faceindex`), or accept a static face.

## 7. Weapon overlay (psprites)

`player->psprites[NUMPSPRITES]` (2 entries) @ d_player.h:156, struct `pspdef_t` @
p_pspr.h. Ticked by p_pspr.c, drawn by `R_DrawPlayerSprites` @ r_things.c:738
iterating `viewplayer->psprites` (r_things.c:761), reading `psp->state->sprite`,
`psp->state->frame` (r_things.c:652-662) and `psp->sx`, `psp->sy` (bob offset).

Transmit per pspr: state index (`psp->state - states`, the p_saveg.c trick @
saveg_write_pspdef_t p_saveg.c:615) + `sx` + `sy`. Null state = not drawn.

## 8. Global scalars

| field | declared | why the client needs it |
|---|---|---|
| `gamestate` | g_game.c:97 (extern doomstat.h:204) | which D_Display branch (we keep only GS_LEVEL) |
| `gametic` | d_loop.c:72 | blink timers in HUD/automap |
| `leveltime` | p_tick.c:27 | client-side `P_UpdateSpecials`-style flat/texture animation (`flattranslation`/`texturetranslation` cycling), so anim tables never ship |
| `paused` | g_game.c:107 | pause "PAUSE" patch overlay |
| `automapactive` | am_map.c:200 | drawer select |
| `menuactive` | m_menu.c:118 | drop if menus are ripped out (menu state is m_menu.c file statics — not worth exporting) |

## Not transmitted (rebuilt or dropped)

- All static geometry: `vertexes`/`segs`/`subsectors`/`nodes` (p_setup.c:52-64),
  line/side/sector static fields — rebuilt by client `P_SetupLevel`.
- Render lookup tables in r_state.h:38-62 (`textureheight`, `spritewidth`,
  `colormaps`, sprite defs) — built from WAD lumps on both sides.
- `texturetranslation`/`flattranslation` (r_state.h:55-56) — client animates from
  `leveltime`.
- Thinker internals, blockmap, RNG (`rndindex` m_random.c) — sim-only.
- Menus, wipe (f_wipe.c), intermission (wi_stuff.c), finale (f_finale.c) — planned
  removal for the one-map port.
- `DG_ScreenBuffer` (doomgeneric.c:21, RESX*RESY*4 malloc) and all of
  i_video.c/v_video.c framebuffers — deleted on the stm side.

## Rough payload (E1M1 full snapshot)

~150 things x ~18B + ~85 sectors x ~14B + view/HUD block (~60B) + sides/lines deltas
(~0 typical) ≈ 4kb raw, before delta encoding and deflate.
