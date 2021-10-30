#ifndef TEST_MENU_HPP
#define TEST_MENU_HPP

#include "Tsukuyomi/ui/Tsukuyomi.hpp"

class Test_Menu {
public:
    static std::shared_ptr<Layout> main_layout;
    static std::shared_ptr<Layout> pannel_layout;

    // BUTTON
    static std::shared_ptr<Layout> button_layout;
    static std::shared_ptr<Button> button;

    // INPUT FORM
    static std::shared_ptr<Layout> form_layout;
    static std::shared_ptr<InputField> input_field;
    static std::shared_ptr<Layout> input_button_layout;
    static std::shared_ptr<Button> input_button;

    // TEXT
    static std::shared_ptr<TextBox> text;
    
    static std::shared_ptr<Menu> menu;

public:
    static const std::shared_ptr<Menu> construct(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler);
};

#endif