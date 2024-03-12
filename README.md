# RP2040 Projects Source Repository

This repository contains all source code for the projects shown on the
[RP2040 projects Youtube channel](https://www.youtube.com/@rp2040projects-bu9xg/videos)

## Compiling the project code

To compile the source code, you first need to clone the [YAHAL library](https://git.fh-aachen.de/Terstegge/YAHAL.git).
Documentation about the prerequisites for this library are still missing,
but you roughly need the same items as for the RPi Pico SDK (a gcc/g++
cross-compiling toolchain for ARM, cmake, ...).

Before compiling the project sources, make sure you set the ```YAHAL_DIR```
environment variable to point to your YAHAL installation. Alternatively you
can set this path in the ```CMakeLists.txt```-file

After these steps, you should be able to compile the project sources. The
build folder will contain a UF2-file, which you can download on your RPi
Pico. If you are using the [RP2040
Launchpad](https://github.com/Terstegge/rp2040-launchpad-PCB), you can use
a make target
```
make upload_<name of the project>
```
to upload the binary file via the picoprobe debugger using OpenOCD.
