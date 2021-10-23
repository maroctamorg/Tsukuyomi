#ifndef GRAPHICS_CONTEXT_HPP
#define GRAPHICS_CONTEXT_HPP

#include "extern.hpp"

class GraphicsContext final {
private:
    int W_W { 800 };
    int W_H { 500 };

public:
    SDL_Window *window { nullptr };
    SDL_Renderer *renderer { nullptr };
    // SDL_Window* getWindow() { return window; }
    // SDL_Renderer* getRenderer() { return renderer; }

    void Init_SDL();
    void Quit_SDL();

public:
    GraphicsContext() {
        Init_SDL();
    }
    GraphicsContext(int width, int height)
        : W_W {width}, W_H {height} {
        Init_SDL();
    }

    ~GraphicsContext() {
        Quit_SDL();
    }

public:
    int getWidth();
    int getHeight();

    void display();
    void updateWindowSize(int w, int h);
};

#endif