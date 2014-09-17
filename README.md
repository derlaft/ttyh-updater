ttyh-updater
============

busybox.exe sh script to update launcher, with WinApi GUI


How to use this trash
=====================

* Find or build busybox.exe and wget.exe binaries. Place them in the root directory.
* Build kostyl and updater-gui "subprojects"
* Place your CA's SSL ca.pem certificate in the root directory (or make corresponding changes in the code)
* Run updater-gui.exe & pray.

Build environment
=================

I'm using MXE toolchain with mingw-w64 to build stuff. You can use any other package that includes mingw-w64 compilers

License && COPYING
=======

License is public domain. For additional information, read DISCLAMER.md file.
