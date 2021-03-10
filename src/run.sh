mrisc32-elf-readelf -sW out/mc1doom | grep FUNC | awk '{print $2,$8}' > /tmp/symbols.csv
mr32sim -g -ga 1073744608 -gp 1073743556 -gd 8 -gw 320 -gh 180 -P /tmp/symbols.csv "$@" out/mc1doom.bin

