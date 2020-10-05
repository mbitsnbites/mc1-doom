![MC1-DOOM Logo](logo/mc1-doom-logo.png)

# MC1-DOOM

This is the classic game DOOM (by id Software) ported to the
[MRISC32](https://mrisc32.bitsnbites.eu/)-based computer
[MC1](https://github.com/mrisc32/mc1).

## Origins & License

MC1-DOOM is based on the original linuxdoom code base from 1997, with some
bugfixes and alterations to make it work on MRISC32, and of course graphics
and I/O routines for the MC1 computer.

Original [README.TXT](iddoc/README.TXT).

The source code is released under the GNU General Public License, as
outlined in [DOOMLIC.TXT](iddoc/DOOMLIC.TXT).

## Building for MC1

In order to build MC1-DOOM for an MC1 target, you need the MRISC32 GNU
toolchain installed. You may find it [here](https://github.com/mrisc32/mrisc32-gnu-toolchain).

```bash
$ cd src
$ make
```

The resulting binary is in placed in `src/out-mc1/`.

## Building for Linux

For testing purposes, you can also build MC1-DOOM for Linux (it requires SDL).

```bash
$ mkdir src/out
$ cd src/out
$ cmake ..
$ cmake --build .
```

