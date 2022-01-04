#ifndef UI_HPP
#define UI_HPP

#include <initializer_list>
#include <memory>
#include <random>
#include "Tsukuyomi.hpp"
#include "circle.hpp"

namespace UI {
    SDL_Color palette[3] {SDL_Color(68, 72, 87, 255), SDL_Color(122, 163, 152, 255), SDL_Color(122, 163, 152, 255)};
};

template <int n>
struct Vertex {
public:
    int connections {0};
    Vertex* edges[n];
    SDL_Point pos;
    SDL_Point vel;
    SDL_Point accel;
    int addConnection(Vertex* vertex) {
        if(connections < 0) return -1;
        else if(connections >= n) {
            edges[n-1] = vertex;
            return n-1;
        }
        else {
            edges[connections] = vertex;
            return connections;
        }
    }
    int removeConnections() {
        for (int i{0}; i < n; i++)
            edges[i] = nullptr;
    }
public:
    Vertex()
        :   pos{rand() % 800, rand() % 600}, vel{rand() % 7 - 3, rand() % 5 - 2}, accel{rand() % 5 - 2, rand() % 3 - 1} {}
    ~Vertex() {
        removeConnections();
    }
};

class Dynamics final : public UI_Element {
private:
    static int counter {0};
    Vertex<10> node_arr[100];
public:
    void render() override {
        for (int i{0}; i < 100; i++) {
            SDL_RenderDrawCircle(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, 10);
            for (int j{0}; j < node_arr[i].connections; j++) {
                SDL_RenderDrawLine(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, node_arr[i].edges[j].pos.x, node_arr[i].edges[j].pos.y);
            }
        }
    }
    void update() override {
        counter++;
        if(counter < 6) return;
        for (int i{0}; i < 100; i++) {
            node_arr[i].pos.x += node_arr[i].vel.x;
            node_arr[i].pos.y += node_arr[i].vel.y;
            node_arr[i].vel.x += node_arr[i].accel.x;
            node_arr[i].vel.y += node_arr[i].accel.y;

            accel.x = rand() % 7 - 3;
            accel.y = rand() % 5 - 2;
        }
        counter = 0;
    }
public:
    Dynamics(const std::shared_ptr<Graphics_Context> context)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()}), loadTexture(context->renderer, "resources/backgroun.jpg")) {
            for (int i{0}; i < 100; i++) {
                for (int j{0}; j < rand() % 11; j++) {
                    node_arr[i].addConnection(&node_arr[rand() % 101]);
                }
            }
        }
    ~Dynamics() = default;
};

Menu* generateOverlay(const std::shared_ptr<Graphics_Context> context) {
    std::shared_ptr<Layout> main_layout = std::make_shared<Layout>(context, std::initializer_list({Container(0.25, 0.25, 0.5, 0.5)}));
    // std::shared_ptr<Layout> pannel_layout = std::make_shared<Layout>(context, std::initializer_list({Container(0.)}));
    // std::shared_ptr<Pannel> pannel = std::make_shared<Pannel>(context)
}

UI_Handler* generateUIHandler() {
    
}

#endif