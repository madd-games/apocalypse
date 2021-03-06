Madd Apocalypse
===============

An advanced general-purpose 3D game engine, including a rendering engine and physics engine,
with an easy-to-use API.

Features
========

* A simple-to-understand yet powerful entity system, useful for creating multiplayer games.
* 3D sound (optional, using OpenAL).
* GPU-accelerated physics and particle systems using OpenCL (optional).

System Requirements
===================

The engine on its own requires a GPU that supports OpenGL 3.3 or newer. Other requirements mostly depend on
the type of game you are trying to run.

Building
========

Building this package requires GCC 4.8.2 at the very least, because some older versions fail to understand all of GCC Vector Extensions in C++.

Currently, all successful builds happened on Linux and Windows systems, targetting Linux and Windows. The building is done by Python using the `build.py` script, which has the following usage:

`build.py [target] [--option=value]`

The 'target' can be any name you choose; multiple different targets means multiple alternative builds (e.g. for different platforms). The following options are supported:

* `--enable-cl` tells the engine to use the EXPERIMENTAL OpenCL support if possible. Currently not recommended.
* `--disable-al` tells the engine not to use OpenAL even if it is available. This will cause sound to be muted. By default, if OpenAL is available, it is used; but if it is not available, the engine will still build without audio support.
* `--host=` specifies what operating system the engine shall be built for. This causes cross-compilation.
* `--no-make` causes all build files to be generated, but `make` will not be ran to build the engine/game. You can still continue by running `make -f build.mk`.

Additionally, `build.py --install` causes `libapocalypse` to be built, and the SDK to be installed. This later allows you to build games in other directories. Currently this is not supported on native Windows builds (but you can still cross-compile to Windows using this feature).

If you want to run the engine on Windows and build from a Linux system, we recommend using the MinGW-W64 cross-compiler:
`i686-w64-mingw32` for 32-bit Windows and `x86_64-w64-mingw32` for 64-bit Windows.

To build the software for Windows (natively or on Linux), you must specify the `sysroot=` option, which points to a directory which must contains 2 subdirectories: `include/SDL2` and `lib`. Also, the tools `tar`, `sed` and `python` (preferably 2.6) must be in the current command search path. MingW-W64 has successfully built both 32-bit and 64-bit versions of the program.

## Build dependencies
To build the engine, you need SDL2, GLEW, OpenGL (which should be installed by default), and optionally an OpenCL SDK and OpenAL.

## Building a game
To build a game on Windows, you must actually copy the `Game` directory to the main directory of the Apocalypse source package and run the build script directly.

To build a game on Linux, you should install the SDK - you can do this by running `./build.py --install`. Next, go to an empty directory, or a directory that already contains a `Game` subdirectory you want to build, and run `apoc-setup-workspace`. You can then build by running `make` - if there is no `Game` directory, then `make` will automatically generate a template for your game.

Passing command-line arguments to `apoc-setup-workspace` causes them to be passed to `build.py` when `make` is ran.
