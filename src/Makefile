##############################################################################
# Makefile for building MC1-DOOM for MC1.
##############################################################################

CC      = mrisc32-elf-gcc
OBJCOPY = mrisc32-elf-objcopy

CFLAGS  = -O3 -std=c11 -W -Wall -Wextra -pedantic -DMC1
LDFLAGS = -msim
LIBS    = -lm

# Set the resolution.
CFLAGS += -DSCREENWIDTH=320 -DSCREENSIZE=200

# Output folder.
OUT = out-mc1

# System specific implementation
SYS_OBJS =				\
		$(OUT)/i_net_dummy.o	\
		$(OUT)/i_sound_dummy.o	\
		$(OUT)/i_system.o	\
		$(OUT)/i_video_dummy.o	\
		$(OUT)/mc1_syscalls.o

OBJS =					\
		$(OUT)/am_map.o	\
		$(OUT)/d_items.o	\
		$(OUT)/d_main.o	\
		$(OUT)/d_net.o		\
		$(OUT)/doomdef.o	\
		$(OUT)/doomstat.o	\
		$(OUT)/dstrings.o	\
		$(OUT)/f_finale.o	\
		$(OUT)/f_wipe.o 	\
		$(OUT)/g_game.o	\
		$(OUT)/hu_lib.o	\
		$(OUT)/hu_stuff.o	\
		$(OUT)/i_main.o	\
		$(OUT)/info.o		\
		$(OUT)/m_argv.o  	\
		$(OUT)/m_bbox.o	\
		$(OUT)/m_cheat.o	\
		$(OUT)/m_fixed.o	\
		$(OUT)/m_menu.o	\
		$(OUT)/m_misc.o	\
		$(OUT)/m_port.o	\
		$(OUT)/m_random.o	\
		$(OUT)/m_swap.o	\
		$(OUT)/p_ceilng.o	\
		$(OUT)/p_doors.o	\
		$(OUT)/p_enemy.o	\
		$(OUT)/p_floor.o	\
		$(OUT)/p_inter.o	\
		$(OUT)/p_lights.o	\
		$(OUT)/p_map.o		\
		$(OUT)/p_maputl.o	\
		$(OUT)/p_mobj.o	\
		$(OUT)/p_plats.o	\
		$(OUT)/p_pspr.o	\
		$(OUT)/p_saveg.o	\
		$(OUT)/p_setup.o	\
		$(OUT)/p_sight.o	\
		$(OUT)/p_spec.o	\
		$(OUT)/p_switch.o	\
		$(OUT)/p_telept.o	\
		$(OUT)/p_tick.o	\
		$(OUT)/p_user.o	\
		$(OUT)/r_bsp.o		\
		$(OUT)/r_data.o	\
		$(OUT)/r_draw.o	\
		$(OUT)/r_main.o	\
		$(OUT)/r_plane.o	\
		$(OUT)/r_segs.o	\
		$(OUT)/r_sky.o		\
		$(OUT)/r_things.o	\
		$(OUT)/sounds.o	\
		$(OUT)/s_sound.o	\
		$(OUT)/st_lib.o	\
		$(OUT)/st_stuff.o	\
		$(OUT)/tables.o	\
		$(OUT)/wi_stuff.o	\
		$(OUT)/w_wad.o		\
		$(OUT)/v_video.o	\
		$(OUT)/z_zone.o	\
		$(SYS_OBJS)

all: $(OUT)/doom.bin

clean:
	rm -f $(OUT)/*

$(OUT)/doom.bin: $(OUT)/doom
	$(OBJCOPY) -O binary out-mc1/doom out-mc1/doom.bin

$(OUT)/doom: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(OUT)/doom $(LIBS)

$(OUT)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

