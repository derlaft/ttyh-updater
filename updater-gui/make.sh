set -e
set -x
i686-w64-mingw32.shared-windres resource.rc res.o
i686-w64-mingw32.shared-gcc -c main.c -o main.o
i686-w64-mingw32.shared-gcc main.o res.o -s -lcomctl32 -Wl,--subsystem,windows -o ../updater-gui.exe
strip ../updater-gui.exe

