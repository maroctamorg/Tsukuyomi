#ifndef UI_HPP
#define UI_HPP

#include <initializer_list>
#include <memory>
#include <random>
#include "Tsukuyomi.hpp"
#include "circle.hpp"

namespace UI {
    SDL_Color palette[3] {SDL_Color(68, 72, 87, 255), SDL_Color(122, 163, 152, 255), SDL_Color(122, 163, 152, 255)};
    std::string font {"resources/fonts/CourierNew.ttf"};
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
    int counter {0};
    Vertex<10> node_arr[100];
public:
    uint iterations {0};
    void render() override {
        for (int i{0}; i < 100; i++) {
            SDL_RenderDrawCircle(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, 10);
            for (int j{0}; j < node_arr[i].connections; j++) {
                SDL_RenderDrawLine(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, node_arr[i].edges[j].pos.x, node_arr[i].edges[j].pos.y);
            }
        }
    }
    void update() override {
        iterations++;
        counter++;
        if(counter < 6) return;
        for (int i{0}; i < 100; i++) {
            node_arr[i].pos.x += node_arr[i].vel.x;
            node_arr[i].pos.y += node_arr[i].vel.y;
            node_arr[i].vel.x += node_arr[i].accel.x;
            node_arr[i].vel.y += node_arr[i].accel.y;

            node_arr[i].accel.x = rand() % 7 - 3;
            node_arr[i].accel.y = rand() % 5 - 2;
        }
        counter = 0;
    }
    void multiplyVelocities(int multiplier) {
        for (int i{0}; i < 100; i++) {
            node_arr[i].vel.x *= multiplier;
            node_arr[i].vel.y *= multiplier;
        }
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

class Button_Press final : public Animation {
private:
    int counter {0};
    const SDL_Color color0;
    const SDL_Color color1 {UI::palette[1]};
    Button& button;
public:
    void next() override {
        if(counter == 0)
            button.setColor(color1);
        if(counter > 6) {
            button.setColor(color0);
            end();
        }
        counter++;
    }
public:
    Button_Press(Button& button) : Animation(false, true), button{button}, color0(button.getColor()) {}
    ~Button_Press() = default;
};

class Overlay final : public UI_Element {
private:
    Dynamics& dynamics;
public:
    std::shared_ptr<Layout> main_layout;
    std::shared_ptr<Layout> pannel_layout;

    std::shared_ptr<Pannel> pannel;

    std::shared_ptr<Text_Box> title_txt;
    std::shared_ptr<Text_Box> counter_txt;

    std::shared_ptr<Input_Field> title_field;
    std::shared_ptr<Input_Field> velocity_field;

    std::shared_ptr<Layout> title_button_layout;
    std::shared_ptr<Button> title_button;
    std::shared_ptr<Layout> velocity_button_layout;
    std::shared_ptr<Button> velocity_button;
    std::shared_ptr<Layout> freeze_button_layout;
    std::shared_ptr<Button> freeze_button;

    std::shared_ptr<Menu> main;

public:
    void render() override {
        main->render();
    }

    void update() override {
        counter_txt->updateText(""+dynamics.iterations);
    }

public:
    Overlay(const std::shared_ptr<Graphics_Context> context, std::shared_ptr<Animation_Handler> animation_handler, std::shared_ptr<Input_Handler> handler, Dynamics& dynamics)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()}), SDL_Color(0, 0, 0, 0)), dynamics(dynamics) {
        // define UI layouts
        main_layout = std::make_shared<Layout>(context, std::initializer_list({Container(0.05, 0.05, 0.1, 0.05), Container(0.25, 0.25, 0.5, 0.5), Container(0.90, 0.05, 0.05, 0.05)}));
        pannel_layout = std::make_shared<Layout>(context, std::initializer_list({Container(0.2, 0.1, 0.6, 0.1), Container(0.2, 0.3, 0.25, 0.1), Container(0.55, 0.3, 0.25, 0.1), Container(0.2, 0.5, 0.25, 0.1), Container(0.55, 0.5, 0.25, 0.1), Container(0.2, 0.6, 0.6, 0.3)}));
        
        //define main pannel and place it into its container
        pannel = std::make_shared<Pannel>(context, pannel_layout, UI::palette[0]);
        main_layout->placeUI_Element(pannel, 1);

        //define and place text_box elements
        title_txt = std::make_shared<Text_Box>(context, "TITLE", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER);
        counter_txt = std::make_shared<Text_Box>(context, "0", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER);
        main_layout->placeUI_Element(title_txt, 0);
        main_layout->placeUI_Element(counter_txt, 2);

        //define and place pannel input fields
        title_field = std::make_shared<Input_Field>(context, handler, 0, UI::font, 15, SDL_Color({0, 0, 0, 255}));
        velocity_field = std::make_shared<Input_Field>(context, handler, 1, UI::font, 15, SDL_Color({0, 0, 0, 255}));
        pannel_layout->placeUI_Element(title_field, 2);
        pannel_layout->placeUI_Element(velocity_field, 3);

        //define and place pannel buttons and button callbacks
        title_button_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) );
        title_button_layout->placeUI_Element(std::make_shared<Text_Box>(context, "CHANGE TITLE", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
        title_button = std::make_shared<Button>(context, handler, title_button_layout, 0, true, false, UI::palette[1]);
        
        velocity_button_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) );
        velocity_button_layout->placeUI_Element(std::make_shared<Text_Box>(context, "VELOCITY MULTIPLIER", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
        velocity_button = std::make_shared<Button>(context, handler, title_button_layout, 1, true, false, UI::palette[1]);

        freeze_button_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) );
        freeze_button_layout->placeUI_Element(std::make_shared<Text_Box>(context, "FREEZE SIMULATION", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
        freeze_button = std::make_shared<Button>(context, handler, title_button_layout, 2, true, false, UI::palette[1]);

        Overlay& overlay(*(this));
        title_button->registerCallBack([animation_handler, overlay](const Graphics_Context& context, const Input_Handler& handler, Button& button) mutable {
            animation_handler->add(std::make_unique<Button_Press>(button));
            overlay.title_txt->updateText(overlay.title_field->getText());
        });
        velocity_button->registerCallBack([animation_handler, overlay, dynamics](const Graphics_Context& context, const Input_Handler& handler, Button& button) mutable {
            animation_handler->add(std::make_unique<Button_Press>(button));
            int multiplier {1};
            try {
                multiplier = std::stoi(overlay.velocity_field->getText());
                dynamics.multiplyVelocities(multiplier);
            } catch (const std::exception& e) {
                std::cout << "Invalid argument...";
            }
        });
        freeze_button->registerCallBack([animation_handler, dynamics](const Graphics_Context& context, const Input_Handler& handler, Button& button) mutable {
            animation_handler->add(std::make_unique<Button_Press>(button));
            dynamics.multiplyVelocities(0);
        });
        
        pannel_layout->placeUI_Element(title_button, 0);
        pannel_layout->placeUI_Element(velocity_button, 1);
        pannel_layout->placeUI_Element(velocity_button, 4);

        //pass pannel into main layout
        main_layout->placeUI_Element(pannel, 1);

        //define main_menu;
        main = std::make_shared<Menu>(context, handler, main_layout, SDL_Color({0, 0, 0, 0}));
    }
};



UI_Handler* generateUIHandler() {
    // implement spacebar callback animation (blocking? probably...) + sound_effect on button_press
    // + background music
}

#endif