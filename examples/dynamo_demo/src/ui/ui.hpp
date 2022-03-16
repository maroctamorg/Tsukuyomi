#ifndef UI_HPP
#define UI_HPP

#include <initializer_list>
#include <memory>
#include <random>
#include "Tsukuyomi.hpp"

namespace UI {
    SDL_Color palette[3] {SDL_Color{68, 72, 87, 255}, SDL_Color{122, 163, 152, 255}, SDL_Color{207, 198, 169, 255}};
    const std::string font {"resources/fonts/CourierNew.ttf"};
    int W_W {800};
    int W_H {600};
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
            connections++;
            return connections;
        }
    }
    void removeConnections() {
        for (int i{0}; i < n; i++)
            edges[i] = nullptr;
    }
public:
    Vertex()
        :   pos{rand() % UI::W_W, rand() % UI::W_H}, vel{rand() % 7 - 3, rand() % 5 - 2}, accel{rand() % 5 - 2, rand() % 3 - 1} {}
    ~Vertex() { 
        removeConnections();
    }
};

class Dynamics final : public UI_Element {
private:
    int counter {0};
    Vertex<10> node_arr[30];
public:
    uint iterations {0};
    void render() override {
        UI_Element::render();
        // SDL_SetRenderDrawColor(context->renderer, UI::palette[1].r, UI::palette[1].g, UI::palette[1].b, UI::palette[1].a);
        // for (int i{0}; i < 30; i++) {
        //     drawCircle(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, 2);
        //     fillCircle(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, 2);
        //     for (int j{0}; j < node_arr[i].connections; j++) {
        //         SDL_RenderDrawLine(context->renderer, node_arr[i].pos.x, node_arr[i].pos.y, node_arr[i].edges[j]->pos.x, node_arr[i].edges[j]->pos.y);
        //     }
        // }
    }
    void update() override {
        iterations++;
        counter++;
        if(counter < 6) return;
        for (int i{0}; i < 30; i++) {
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
        for (int i{0}; i < 30; i++) {
            node_arr[i].vel.x *= multiplier;
            node_arr[i].vel.y *= multiplier;
        }
    }
public:
    Dynamics(const std::shared_ptr<Graphics_Context> context)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()}), "resources/background.jpg") {
            std::cout << "Constructing dynamics...\n";
            for (int i{0}; i < 30; i++) {
                for (int j{0}; j < rand() % 11; j++) {
                    node_arr[i].addConnection(node_arr + (rand() % 101));
                }
            }
        }
    ~Dynamics() = default;
};

class Button_Press final : public Animation {
private:
    int counter {0};
    const SDL_Color color0;
    static constexpr SDL_Color color1 {122, 163, 255, 255};
    std::weak_ptr<Button> w_button;
public:
    void start() override {
        this->Animation::start();
        if(auto button = w_button.lock()) button->setColor(color1);
        else std::cout << "Unable to lock button in Button_Press animation...\n";
    }
    void next() override {
        if(counter == 10) {
            if(auto button = w_button.lock()) {
                button->setColor(color0);
                end();  
            } else {
                std::cout << "Unable to lock button in Button_Press animation...\n";
            }
        }
        counter++;
    }
public:
    Button_Press(std::shared_ptr<Button>& button) : Animation(false, true), w_button{button}, color0(button->getColor()) {}
    ~Button_Press() = default;
};

class Overlay final : public UI_Element {
private:
    std::weak_ptr<Dynamics> dynamics;
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
        if(const auto dyn = dynamics.lock())
            counter_txt->updateText(std::to_string(dyn->iterations));
        else
            std::cout << "Unable to lock dynamics pointer in Overlay::update()\n";
        // main->update();
    }
    
    void updatePosition(const SDL_Rect& rect) override {
        // this->UI_Element::updatePosition(rect);
        main->updateSize();
    }

public:
    Overlay(const std::shared_ptr<Graphics_Context> context, std::shared_ptr<Animation_Handler> a_handler, std::shared_ptr<Input_Handler> i_handler, std::shared_ptr<Dynamics> dynamics)
        :   UI_Element(context, SDL_Rect({0, 0, context->getWidth(), context->getHeight()}), SDL_Color{0, 0, 0, 0}, SDL_Color{0, 0, 0, 0}, true), dynamics(dynamics) {
        std::cout << "Constructing overlay...\n";
        // define UI layouts
        main_layout = std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.05, 0.05, 0.1, 0.05), Container(0.15, 0.15, 0.7, 0.7), Container(0.90, 0.05, 0.05, 0.05)}));
        pannel_layout = std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.2, 0.1, 0.6, 0.1), Container(0.2, 0.3, 0.25, 0.1), Container(0.55, 0.3, 0.25, 0.1), Container(0.2, 0.5, 0.25, 0.1), Container(0.55, 0.5, 0.25, 0.1), Container(0.2, 0.7, 0.6, 0.2)}));

        //define and place text_box elements
        title_txt = std::make_shared<Text_Box>(context, "SIMULATION", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER, UI::palette[0]);
        counter_txt = std::make_shared<Text_Box>(context, "0", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER, UI::palette[0]);
        main_layout->placeUI_Element(title_txt, 0);
        main_layout->placeUI_Element(counter_txt, 2);
        pannel_layout->placeUI_Element(std::make_shared<Text_Box>(context, "CONTROLS", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER, SDL_Color{255, 255, 255, 255}), 0);

        //define and place pannel input fields
        title_field = std::make_shared<Input_Field>(context, i_handler, 0, UI::font, 12, SDL_Color{0, 0, 0, 255});
        velocity_field = std::make_shared<Input_Field>(context, i_handler, 1, UI::font, 12, SDL_Color{0, 0, 0, 255});
        pannel_layout->placeUI_Element(title_field, 2);
        pannel_layout->placeUI_Element(velocity_field, 4);

        //define and place pannel buttons and button callbacks
        title_button_layout = std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.1, 0.05, 0.8, 0.9)}));
        title_button_layout->placeUI_Element(std::make_shared<Text_Box>(context, "CHANGE TITLE", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
        title_button = std::make_shared<Button>(context, i_handler, title_button_layout, 0, true, false, UI::palette[1]);
        
        velocity_button_layout = std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.1, 0.05, 0.8, 0.9)}));
        velocity_button_layout->placeUI_Element(std::make_shared<Text_Box>(context, "VELOCITY MULTIPLIER", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
        velocity_button = std::make_shared<Button>(context, i_handler, velocity_button_layout, 1, true, false, UI::palette[1]);

        freeze_button_layout = std::make_shared<Layout>(context, std::initializer_list<Container>({Container(0.1, 0.05, 0.8, 0.9)}));
        freeze_button_layout->placeUI_Element(std::make_shared<Text_Box>(context, "FREEZE SIMULATION", UI::font, 15, UI::palette[2], ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
        freeze_button = std::make_shared<Button>(context, i_handler, freeze_button_layout, 2, true, false, UI::palette[1]);

        uint index = a_handler->add(std::make_unique<Button_Press>(title_button));
        title_button->registerCallBack([index, a_handler, overlay{this}]() mutable {
            a_handler->start(index);
            overlay->title_txt->updateText(overlay->title_field->getText());
        });
        index = a_handler->add(std::make_unique<Button_Press>(velocity_button));
        velocity_button->registerCallBack([index, a_handler, overlay{this}]() mutable {
            a_handler->start(index);
            int multiplier {1};
            try {
                multiplier = std::stoi(overlay->velocity_field->getText());
                if(auto ptr_dynamics = overlay->dynamics.lock()) ptr_dynamics->multiplyVelocities(multiplier);
                else std::cout << "Unable to lock pointer to dynamics in velocity button callback...";
            } catch (...) {
                std::cout << "Invalid argument...\n";
            }
        });
        index = a_handler->add(std::make_unique<Button_Press>(freeze_button));
        freeze_button->registerCallBack([index, a_handler, overlay{this}]() mutable {
            a_handler->start(index);
            if(auto ptr_dynamics = overlay->dynamics.lock()) ptr_dynamics->multiplyVelocities(0);
            else std::cout << "Unable to lock pointer to dynamics in freeze button callback...";
        });
        
        pannel_layout->placeUI_Element(title_button, 1);
        pannel_layout->placeUI_Element(velocity_button, 3);
        pannel_layout->placeUI_Element(freeze_button, 5);

        //define main pannel
        pannel = std::make_shared<Pannel>(context, pannel_layout, UI::palette[0]);
        //pass pannel into main layout
        main_layout->placeUI_Element(pannel, 1);

        //define main_menu;
        main = std::make_shared<Menu>(context, main_layout, SDL_Color{0, 0, 0, 0});
    }
};

class Overlay_Show_Animation final : public Animation {
private:
    const uint steps {10000};
    const int index;
    int counter {0};
    Container final_pos;
    Container current;
    std::shared_ptr<Layout> layout;
    std::shared_ptr<Overlay> overlay;
public:
    void start() override {
        this->Animation::start();
        final_pos = layout->getContainer(index);
        current = final_pos;
        current.r_w = final_pos.r_w / steps;
        current.r_h = final_pos.r_h / steps;
        layout->addContainer(index, current);
        overlay->setHidden(false);
        std::cout << "Started Show Overlay Animation...\n";
    }
    void next() override {
        if(counter == steps) {
            layout->addContainer(index, final_pos);
            end();
            std::cout << "Ended Show Overlay Animation...\n";
        } else if ((steps - counter) % 10 == 0) {
            current.r_w = final_pos.r_w / ((steps - counter) / 10 + 1);
            current.r_h = final_pos.r_h / ((steps - counter) / 10 + 1);
            layout->addContainer(index, current);
        }
        counter++;
    }
public:
    Overlay_Show_Animation(std::shared_ptr<Overlay> overlay, std::shared_ptr<Layout> layout, int index) : Animation(true, true), layout{layout}, index{index}, overlay{overlay} {}
    ~Overlay_Show_Animation() = default;
};

class Overlay_Hide_Animation final : public Animation {
private:
    const uint steps {10000};
    const int index;
    int counter {0};
    Container final_pos;
    Container current;
    std::shared_ptr<Layout> layout;
    std::shared_ptr<Overlay> overlay;
public:
    void start() override {
        this->Animation::start();
        final_pos = layout->getContainer(index);
        current = final_pos;
        std::cout << "Started Hide Overlay Animation...\n";
    }
    void next() override {
        if(counter == steps) {
            overlay->setHidden(true);
            layout->addContainer(index, final_pos);
            end();
            std::cout << "Ended Hide Overlay Animation...\n";
        } else if (counter % 10 == 0) {
            current.r_w = final_pos.r_w / (counter / 10 + 1);
            current.r_h = final_pos.r_h / (counter / 10 + 1);
            layout->addContainer(index, current);
        }
        counter++;
    }
public:
    Overlay_Hide_Animation(std::shared_ptr<Overlay> overlay, std::shared_ptr<Layout> layout, int index) : Animation(true, true), layout{layout}, index{index}, overlay{overlay} {}
    ~Overlay_Hide_Animation() = default;
};

#endif