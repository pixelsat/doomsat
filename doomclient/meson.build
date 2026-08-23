project(
  'doomclient',
  'c',
  version: '0.1.0',
  license: 'GPL-2.0-or-later',
  default_options: [
    'c_std=c99',
    'warning_level=1',
    'optimization=s',
    'debug=true',
  ],
)

cc = meson.get_compiler('c')

doom_core = files(
  'am_map.c',
  'd_event.c',
  'd_items.c',
  'd_iwad.c',
  'd_loop.c',
  'd_main.c',
  'd_mode.c',
  'd_net.c',
  'doomdef.c',
  'doomgeneric.c',
  'doomstat.c',
  'dstrings.c',
  'dummy.c',
  'f_finale.c',
  'f_wipe.c',
  'g_game.c',
  'hu_lib.c',
  'hu_stuff.c',
  'i_cdmus.c',
  'i_endoom.c',
  'i_input.c',
  'i_joystick.c',
  'i_scale.c',
  'i_sound.c',
  'i_system.c',
  'i_timer.c',
  'i_video.c',
  'info.c',
  'm_argv.c',
  'm_bbox.c',
  'm_cheat.c',
  'm_config.c',
  'm_controls.c',
  'm_fixed.c',
  'm_menu.c',
  'm_misc.c',
  'm_random.c',
  'memio.c',
  'p_ceilng.c',
  'p_doors.c',
  'p_enemy.c',
  'p_floor.c',
  'p_inter.c',
  'p_lights.c',
  'p_map.c',
  'p_maputl.c',
  'p_mobj.c',
  'p_plats.c',
  'p_pspr.c',
  'p_saveg.c',
  'p_setup.c',
  'p_sight.c',
  'p_spec.c',
  'p_switch.c',
  'p_telept.c',
  'p_tick.c',
  'p_user.c',
  'r_bsp.c',
  'r_data.c',
  'r_draw.c',
  'r_main.c',
  'r_plane.c',
  'r_segs.c',
  'r_sky.c',
  'r_things.c',
  's_sound.c',
  'sha1.c',
  'sounds.c',
  'st_lib.c',
  'st_stuff.c',
  'statdump.c',
  'tables.c',
  'v_video.c',
  'w_checksum.c',
  'w_file.c',
  'w_file_stdc.c',
  'w_main.c',
  'w_wad.c',
  'wi_stuff.c',
  'z_zone.c',
)

backend = get_option('video_backend')

doom_args = [
  '-DNORMALUNIX',
  '-DLINUX',
  '-DSNDSERV',
  '-D_DEFAULT_SOURCE',
]

doom_deps = [cc.find_library('m', required: false)]
backend_sources = []

if backend == 'sdl'
  backend_sources += files(
    'doomgeneric_sdl.c',
    'i_sdlmusic.c',
    'i_sdlsound.c',
    'mus2mid.c',
  )
  doom_args += '-DFEATURE_SOUND'
  doom_deps += dependency('sdl2')
  doom_deps += dependency('SDL2_mixer')
elif backend == 'xlib'
  backend_sources += files('doomgeneric_xlib.c')
  doom_deps += dependency('x11')
endif

doom_args += cc.get_supported_arguments(
  '-Wno-unused-variable',
  '-Wno-unused-but-set-variable',
  '-Wno-unused-function',
  '-Wno-missing-braces',
  '-Wno-format-truncation',
  '-Wno-stringop-truncation',
  '-Wno-stringop-overflow',
  '-Wno-implicit-fallthrough',
)

executable(
  'doomclient',
  doom_core + backend_sources,
  c_args: doom_args,
  dependencies: doom_deps,
  link_args: ['-Wl,--gc-sections'],
  install: false,
)
