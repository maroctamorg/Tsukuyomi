#ifndef GRAPHICS_EXTERN_HPP
#define GRAPHICS_EXTERN_HPP

#include <iostream>
#include <utility>
#include <memory>
#include <string>
#include <array>
#include <vector>
#ifdef __linux__
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
    #include <SDL2_mixer/SDL_mixer.h>
#endif

#endif