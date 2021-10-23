#include "rectangles.hpp"
#include <iostream>

void drawBaseRects (SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r) {
    SDL_Rect core { rect.x + r, rect.y + r, rect.w - 2*r, rect.h - 2*r };
    SDL_Rect left { rect.x, rect.y + r, r, rect.h - 2*r };
    SDL_Rect right { rect.x + rect.w - r, rect.y + r, r, rect.h - 2*r };
    SDL_Rect top { rect.x + r, rect.y, rect.w - 2*r, r };
    SDL_Rect bottom { rect.x + r, rect.y + rect.h - r, rect.w - 2*r, r };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &core);
    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);
    SDL_RenderFillRect(renderer, &top);
    SDL_RenderFillRect(renderer, &bottom);

    if(border_color.a != 0) {
        // DRAW BORDER
        SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
        SDL_RenderDrawLine(renderer, left.x, left.y, left.x, left.y + left.h);
        SDL_RenderDrawLine(renderer, right.x + right.w, right.y, right.x + right.w, right.y + right.h);
        SDL_RenderDrawLine(renderer, top.x, top.y, top.x + top.w, top.y);
        SDL_RenderDrawLine(renderer, bottom.x, bottom.y + bottom.h, bottom.x + bottom.w, bottom.y + bottom.h);
    }
}

void drawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r, bool shadow) {
    if(r > 8)
        r = 8;
    if(shadow) {
        // drawRoundedRect(renderer, {rect.x - static_cast<int>(rect.w/10), rect.y - static_cast<int>(rect.h/20), rect.w, rect.h}, {0, 0, 0, 200}, {0, 0, 0, 0}, r, false);
        drawRoundedRect(renderer, {rect.x - 10, rect.y + 10, rect.w, rect.h}, {0, 0, 0, 100}, {0, 0, 0, 0}, r, false);
    }
    drawBaseRects(renderer, rect, color, border_color, r);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    auto drawPoints = [renderer, rect, color, border_color, r](int r_x, int r_y) {
        // std::cout << "Call to drawPoints:\t" << r_x << "\t" << r_y << "\n";
        SDL_RenderDrawPoint(renderer, rect.x + rect.w - r + r_x, rect.y + r - r_y - 1);         // FIRST QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + r - r_x - 1, rect.y + r  - r_y - 1);             // SECOND QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + r - r_x - 1, rect.y + rect.h - r + r_y);         // THIRD QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + rect.w - r + r_x, rect.y + rect.h - r + r_y);    // FOURTH QUADRANT
    };

    for(int r_x {0}; r_x < r; r_x++) {
        for(int r_y {0}; r_y < r; r_y++) {
            if(pow(r_x,2) + pow(r_y,2) <= pow(r,2)) {
                drawPoints(r_x, r_y);
            }
        }
    }
}

void drawCutRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r, bool shadow) {
    drawBaseRects(renderer, rect, color, border_color, r);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    auto drawPoints = [renderer, rect, color, border_color, r](int r_x, int r_y) {
        // std::cout << "Call to drawPoints:\t" << r_x << "\t" << r_y << "\n";
        SDL_RenderDrawPoint(renderer, rect.x + rect.w - r + r_x, rect.y + r - r_y - 1);             // FIRST QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + r - r_x - 1, rect.y + r  - r_y - 1);                      // SECOND QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + r - r_x - 1, rect.y + rect.h - r + r_y);             // THIRD QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + rect.w - r + r_x, rect.y + rect.h - r + r_y);    // FOURTH QUADRANT
    };

    int y_bound { 0 };
    auto getBound = [r, &y_bound](int r_x, int r_y) {
        y_bound = static_cast<int>(sqrt(pow(r-1, 2)-pow(r_x,2)));
        return y_bound;
    };

    int r_x {0}, r_y {r - 1};
    while(r_x < r) {
        while(r_y >= 0) {
            drawPoints(r_x, r_y);
            r_y--;
        }
        r_y = r - 1 - r_x;
        r_x++;
    }
}