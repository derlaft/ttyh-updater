set -e
set -x

x86_64-w64-mingw32.static-gcc kostyl.c -o ../x86_64-kostyl.exe
i686-w64-mingw32.static-gcc kostyl.c -o ../i686-kostyl.exe
strip ../x86_64-kostyl.exe
strip ../i686-kostyl.exe
