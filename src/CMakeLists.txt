# -*- mode: CMake; tab-width: 2; indent-tabs-mode: nil; -*-

cmake_minimum_required(VERSION 3.10)

project(mc1-doom)

set(SRCS
		am_map.c
		d_items.c
		d_main.c
		d_net.c
		doomdef.c
		doomstat.c
		dstrings.c
		f_finale.c
		f_wipe.c
		g_game.c
		hu_lib.c
		hu_stuff.c
		i_main.c
		info.c
		i_system.c
		m_argv.c
		m_bbox.c
		m_cheat.c
		m_fixed.c
		m_menu.c
		m_misc.c
		m_random.c
		m_swap.c
		p_ceilng.c
		p_doors.c
		p_enemy.c
		p_floor.c
		p_inter.c
		p_lights.c
		p_map.c
		p_maputl.c
		p_mobj.c
		p_plats.c
		p_pspr.c
		p_saveg.c
		p_setup.c
		p_sight.c
		p_spec.c
		p_switch.c
		p_telept.c
		p_tick.c
		p_user.c
		r_bsp.c
		r_data.c
		r_draw.c
		r_main.c
		r_plane.c
		r_segs.c
		r_sky.c
		r_things.c
		sounds.c
		s_sound.c
		st_lib.c
		st_stuff.c
		tables.c
		wi_stuff.c
		w_wad.c
		v_video.c
		z_zone.c)

set(LIBS "")

# Video.
find_package(SDL)
if(SDL_FOUND)
  list(APPEND SRCS i_video_sdl.c)
  list(APPEND LIBS ${SDL_LIBRARY})
else()
  list(APPEND SRCS i_video_dummy.c)
endif()

# Sound.
# TODO(m): Add something that works, e.g. ALSA.
list(APPEND SRCS i_sound_dummy.c)

# Network.
list(APPEND SRCS i_net.c)

add_executable(mc1doom ${SRCS})
target_compile_definitions(mc1doom PRIVATE -DNORMALUNIX -DLINUX)
target_link_libraries(mc1doom PRIVATE ${LIBS})

