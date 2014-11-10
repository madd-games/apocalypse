Madd Apocalypse
===============

An advanced general-purpose 3D game engine, including a rendering engine and physics engine,
with an easy-to-use API.

Features
========

* Custom maps, weapons, shaders, etc.
* Singleplayer and multiplayer, including the possibility to build a dedicated server.
* 3D sound (optional, using OpenAL).
* GPU-accelerated physics and particle systems using OpenCL (optional).

System Requirements
===================

The engine on its own requires a GPU that supports OpenGL 3.3 or newer. Other requirements mostly depend on
the type of game you are trying to run. We also recommend a GPU that supports OpenCL (any version), as this
is used to optimize the engine.

Building
========

Currently, all successful builds happened on a Linux system, targetting Linux and Windows. The building is done
by Python using the `build.py` script, which has the following usage:

`build.py [target] [--option=value]`

The only target supported right now is `client`, which is also the default. There are a few available options:

* `--enable-cl` tells the engine to use the EXPERIMENTAL OpenCL support if possible. Currently not recommended.
* `--disable-al` tells the engine not to use OpenAL even if it is available. This will cause sound to be muted. By default, if OpenAL is available, it is used; but if it is not available, the engine will still build without audio support.
* `--host=` specifies what operating system the engine shall be built for. This causes cross-compilation.

If you want to run the engine on Windows and build from a Linux system, we recommend using the MinGW-W64 cross-compiler:
`i686-w64-mingw32` for 32-bit Windows and `x86_64-w64-mingw32` for 64-bit Windows.

## Build dependencies
To build the engine, you need SDL2, GLEW, OpenGL (which should be installed by default), and optionally an OpenCL SDK and OpenAL.

## Building a game
To build a game, you should install the SDK - you can do this by running the `install-sdk` script in its directory. Next, go to an empty directroy, or a directory that already contains a `Game` subdirectroy you want to build, and run `apoc-setup-workspace`. You can thenbuild by running `make` - if there is no `Game` directory, then `make` will automatically generate a template for your game. You can also edit your `Makefile` to add more options to `build.py` as shown above.
