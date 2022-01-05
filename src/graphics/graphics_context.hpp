#ifndef GRAPHICS_CONTEXT_HPP
#define GRAPHICS_CONTEXT_HPP

#include "extern.hpp"

class Graphics_Context final {
private:
    int W_W { 800 };
    int W_H { 500 };
    std::string title;

public:
    SDL_Window *window { nullptr };
    SDL_Renderer *renderer { nullptr };
    // SDL_Window* getWindow() { return window; }
    // SDL_Renderer* getRenderer() { return renderer; }

    void Init_SDL();
    void Quit_SDL();

public:
    Graphics_Context(std::string Title)
        : title{Title} {
        Init_SDL();
    }
    Graphics_Context(int width, int height, std::string Title)
        : title{Title}, W_W {width}, W_H {height} {
        Init_SDL();
    }

    ~Graphics_Context() {
        Quit_SDL();
    }

public:
    int getWidth();
    int getHeight();

    void display();
    void updateWindowSize(int w, int h);
};

#endif