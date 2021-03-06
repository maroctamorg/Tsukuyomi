TSUKUYOMI
=========
A C++ lightweight, customizable, cross-platform GUI library
---------------------------------------------------

Tsukuyomi is a light-weight, highly-customizable GUI library which aims to make C++ GUI development simple and flexible.

Using the built-in UI_Element-derived classes, a developer can implement a simple GUI in only a few lines of code. For more advanced developers, or specific use-cases, it is also quite straightforward to develop your own UI_Elements by standard inheritance.

Built on SDL2, the library can support a wide array of target systems, though at this moment of very early development it works out of the box only on macOS and Linux.

----------------------
Dependencies
------------

- **SDL2** <br> (https://www.libsdl.org/)
- **SDL_image** <br> (https://www.libsdl.org/projects/SDL_image/)
- **SDL_ttf** <br> (https://www.libsdl.org/projects/SDL_ttf/)
- **SDL_mixer** <br> (https://www.libsdl.org/projects/SDL_mixer/)

----------------------
Support
------------
Due to issues in linking with the SDL_image and SDL_mixer frameworks, the library does not yet run natively on apple silicon; the provided Makefile cross-compiles to x86_64 which can then be run via Rosetta2 translation; see examples for cross-compilation to x86_64 on arm macs.

It should be straightforward to port it to other platforms, such as iOS, Windows, etc...; this will be done in the future.

----------------------
Installation
------------

There are a few alternatives for installation; you may also wish to use the library without installing it.
In any case, you may wish to clone the repository to your machine
```bash
git clone https://github.com/maroctamorg/Tsukuyomi
```

**1. Making use of the macOS installation script**

After cloning the repository, simply copy the installation script to the project root and run it
```bash
cp installation_scripts/install_macOS.sh install.sh
bash install.sh
```

This will download and install the necessary SDL frameworks to ```${HOME}/Library/Frameworks/```, build a static library ```Tsukuyomi.a``` and install it (together with the header files) to ```${HOME}/Library/Tsukuyomi```

(*) *Note: on macOS, you may need to add the path ```${HOME}/Library/Frameworks``` to the shell variable ```DYLD_FALLBACK_FRAMEWORK_PATH``` (in case you encounter linker errors with SDL when executing the examples); if you do so, it might also be necessary to add paths ```/Library/Frameworks``` and ```/System/Library/Frameworks``` to the variable ```DYLD_FRAMEWORK_PATH``` in case they are not already present (you can check this by echoing the variable).*

*For a single shell session, this may be achieved by issuing the following commands*
```bash
export DYLD_FALLBACK_FRAMEWORK_PATH=$DYLD_FALLBACK_FRAMEWORK_PATH:${HOME}/Library/Frameworks
export DYLD_FRAMEWORK_PATH=$DYLD_FRAMEWORK_PATH:System/Library/Frameworks:/Library/Frameworks
```

This can be made persistent by adding the above lines to your shell profile (```${HOME}\.bashrc``` or ```${HOME}\.zshrc``` for bash and zsh respectively).

**2. Downloading the latest pre-built release from GitHub**

If available, you may also download the latest pre-built release from GitHub for your platform: this will include the ```Tsukuyomi.a``` static library file, as well as the required header files in a folder ```include``` and a ```info.txt``` file specifying the library version, target platform and dependencies version; copy these into the desired install location.

**3. Building from source**

If you have a standard installation of SDL2 on macOS (```/Library/Frameworks``` or ```${HOME}/Library/Frameworks```) or linux (via your distribution's package manager, and with sdl2-config), one of the provided Makefiles should be sufficient. Just copy the appropriate file from the ```Makefiles``` folder to the root of the project and run 
```bash
cp Makefiles/Makefile_${PLATFORM} Makefile
make
```

This will produce a static library ```Tsukuyomi.a```, in the build directory. You may then copy this file, and the ```.hpp``` files in the src folder onto the directory of your choice (maintaining the directory structure); the following commands would do the trick
```bash
cp build/Tsukuyomi.a $INSTALL_PATH/
mkdir $INSTALL_PATH/include
cd src/;
find . -name '*.hpp' | cpio -updm $INSTALL_PATH/include/;
```

For a non-standard installation of SDL2, make sure to update the Makefile variables ```${INCLUDE}```, ```${FRAMEWORKS}``` and ```${LINKER_FLAGS}``` accordingly.
Also make sure to specify your compiler (tested with clang++ and gcc) by updating the Makefile variable ```${CC}```.

----------------------
Usage
------------

In order to use the framework in your own apps, you must
```c++
#include "Tsukuomi.hpp"
```
in your code; point the compiler to the include directory and link against the static library file ```Tsukuyomi.a``` and the SDL dependencies.

The ```examples``` folder contains simple programs and makefiles that can be used as reference; you may test the correct installation of the library by building the sample programs; there are makefiles provided both linking directly to the build output of the root project and to the standard installation of the library.

These are mostly meant as tests however; for an example of an actual application, check the (work-in-progress) repository: https://github.com/maroctamorg/Pong.

----------------------
Notes
------------

This library is still in early development and not fit for distribution, significant refactorings are expected and documentation is not yet available.

If you'd like to contribute or are simply curious, check out the TODO.md document and feel free to reach out to my email guimaraesneto.marcelo@gmail.com.

<!-- License
-------
The library and its source code are distributed under the [GPLv3 license](https://opensource.org/licenses/GPL-3.0). -->
