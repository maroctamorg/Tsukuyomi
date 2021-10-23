#ifndef TEST_MENU_HPP
#define TEST_MENU_HPP

#include "ui.hpp"
#include "../services/client.hpp"

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
    
    static std::shared_ptr<Menu> menu;

public:
    static const std::shared_ptr<Menu> construct(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::weak_ptr<CustomClient> connection);
};

#endif