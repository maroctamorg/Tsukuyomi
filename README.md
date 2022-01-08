TSUKUYOMI
=========
A C++ lightweight, customizable, cross-platform GUI framework
---------------------------------------------------

Tsukuyomi is a light-weight, highly-customizable GUI framework which aims to make C++ GUI development simple and flexible.

Using the built-in UI_Element-derived classes, a developer can implement a simple, yet fully-featured GUI in only a few lines of code. For more advanced developers, or specific use-cases, it is also quite straightforward to develop your own UI_Elements by standard inheritance.

Built on SDL2, the framework can support a wide array of target systems, though at this moment of very early development it works out of the box only on macOS and Linux.

----------------------
Dependencies
------------

- **SDL2**  (https://www.libsdl.org/)

----------------------
Installation
------------

**Making use of the installation script**

The installation script is not yet complete, 

**Building the Framework from source as a static library**

For a standard installation of SDL2 on macOS or linux, one of the provided Makefiles should be sufficient. Just copy the appropriate file from the ```Makefiles``` folder, to the root of the project and run 
```bash
make
```
This will produce a static library ```Tsukuyomi.a```, in the build directory. You may then copy this file, and the .hpp files in the src folder (maintaining the directory structure) onto the installation location of your choice.

In order to use the framework in your own apps, you must
```c++
#include "Tsukuomi.hpp"
```
in your code, point the compiler to path of the .hpp files and link against the static library file.

For a non-standard installation of SDL2, make sure to udpate the Makefile variables ```${INCLUDE}```, ```${FRAMEWORKS}``` and ```${LINKER_FLAGS}``` accordingly.

Also make sure to specify your compiler (tested with clang++ and gcc) by updating the Makefile variable ```${CC}```.

<!-- License -->
-------
