#include "rectangles.hpp"
#include <iostream>

void drawBaseRects (SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r, bool shadow) {
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
        SDL_RenderDrawLine(renderer, left.x - 1, left.y - 1, left.x - 1, left.y + left.h);
        SDL_RenderDrawLine(renderer, right.x + right.w, right.y - 1, right.x + right.w, right.y + right.h);
        SDL_RenderDrawLine(renderer, top.x - 1, top.y - 1, top.x + top.w, top.y - 1);
        SDL_RenderDrawLine(renderer, bottom.x - 1, bottom.y + bottom.h, bottom.x + bottom.w, bottom.y + bottom.h);
        // DRAW SHADOW
        if(shadow) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_RenderDrawLine(renderer, left.x - 2, left.y - 2, left.x - 2, left.y + left.h + 1);
            SDL_RenderDrawLine(renderer, right.x + right.w + 1, right.y - 2, right.x + right.w + 1, right.y + right.h + 1);
            SDL_RenderDrawLine(renderer, top.x - 2, top.y - 2, top.x + top.w + 1, top.y - 2);
            SDL_RenderDrawLine(renderer, bottom.x - 2, bottom.y + bottom.h + 1, bottom.x + bottom.w + 1, bottom.y + bottom.h + 1);
            
            SDL_RenderDrawLine(renderer, left.x - 3, left.y - 3, left.x - 3, left.y + left.h + 2);
            SDL_RenderDrawLine(renderer, right.x + right.w + 2, right.y - 3, right.x + right.w + 2, right.y + right.h + 2);
            SDL_RenderDrawLine(renderer, top.x - 3, top.y - 3, top.x + top.w + 2, top.y - 3);
            SDL_RenderDrawLine(renderer, bottom.x - 3, bottom.y + bottom.h + 2, bottom.x + bottom.w + 2, bottom.y + bottom.h + 2);
        }
    } else {
        if(shadow) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_RenderDrawLine(renderer, left.x - 1, left.y - 1, left.x - 1, left.y + left.h);
            SDL_RenderDrawLine(renderer, right.x + right.w, right.y - 1, right.x + right.w, right.y + right.h);
            SDL_RenderDrawLine(renderer, top.x - 1, top.y - 1, top.x + top.w, top.y - 1);
            SDL_RenderDrawLine(renderer, bottom.x - 1, bottom.y + bottom.h, bottom.x + bottom.w, bottom.y + bottom.h);

            SDL_RenderDrawLine(renderer, left.x - 2, left.y - 2, left.x - 2, left.y + left.h + 1);
            SDL_RenderDrawLine(renderer, right.x + right.w + 1, right.y - 2, right.x + right.w + 1, right.y + right.h + 1);
            SDL_RenderDrawLine(renderer, top.x - 2, top.y - 2, top.x + top.w + 1, top.y - 2);
            SDL_RenderDrawLine(renderer, bottom.x - 2, bottom.y + bottom.h + 1, bottom.x + bottom.w + 1, bottom.y + bottom.h + 1);
        }
    }
}

void drawRoundedRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r, bool shadow) {
    if(r > 8)
        r = 8;
    drawBaseRects(renderer, rect, color, border_color, r, shadow);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    auto drawPoints = [renderer, rect, color, border_color, r](int r_x, int r_y) {
        // std::cout << "Call to drawPoints:\t" << r_x << "\t" << r_y << "\n";
        SDL_RenderDrawPoint(renderer, rect.x + rect.w - r + r_x, rect.y + r - r_y - 1);         // FIRST QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + r - r_x - 1, rect.y + r  - r_y - 1);             // SECOND QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + r - r_x - 1, rect.y + rect.h - r + r_y);         // THIRD QUADRANT
        SDL_RenderDrawPoint(renderer, rect.x + rect.w - r + r_x, rect.y + rect.h - r + r_y);    // FOURTH QUADRANT
    };

    int y_bound { 0 };
    auto getBound = [r, &y_bound](int r_x, int r_y) {
        y_bound = static_cast<int>(sqrt(pow(r - 1, 2)-pow(r_x,2)));
        return y_bound;
    };

    // std::cout << "############## Drawing rounded corner pixels\n ##############";
    int r_x { 0 }, r_y { 0 };
    while (r_x < r+1) {
        while(r_y < getBound(r_x, r_y) + 1) {
            drawPoints(r_x, r_y);
            r_y++;
        }
        if(border_color.a != 0) {
            SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
            drawPoints(r_x, r_y);
            r_y++;
        }
        if(shadow) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            drawPoints(r_x, r_y);
            r_x++;
            drawPoints(r_x, r_y);
            r_x--;
            r_y++;
            drawPoints(r_x, r_y);
            r_x++;
            drawPoints(r_x, r_y);
        } else {
            r_x++;
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        r_y = 0;
    }
    // int r_x {0}, r_y {r - 1};
    // while(r_x < r) {
    //     while(r_y >= 0) {
    //         drawPoints(r_x, r_y);
    //         r_y--;
    //     }
    //     r_y = r - 1 - r_x;
    //     r_x++;
    // }
    // int r_x, r_y;
    // for(float t = 0; t <= M_PI/2; t+=0.001) {
    //     for(int a = 0; a < r; a++) {
    //         r_x = a*cos(t);
    //         r_y = a*sin(t);
    //         drawPoints(r_x, r_y);
    //     }
    //     if(border_color.a != 0) {
    //         r_x = r*cos(t);
    //         r_y = r*sin(t); 
    //         SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
    //         drawPoints(r_x, r_y);
    //         SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    //     }
        
    // }

    // std::cout << "############## Finished drawing rounded corner pixels\n ##############";
}

void drawCutRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, const SDL_Color& border_color, int r, bool shadow) {
    drawBaseRects(renderer, rect, color, border_color, r, false);
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