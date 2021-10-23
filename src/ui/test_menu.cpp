#include "test_menu.hpp"
#include <initializer_list>

std::shared_ptr<Layout>      Test_Menu::main_layout          { nullptr };
std::shared_ptr<Layout>      Test_Menu::pannel_layout        { nullptr };

// BUTTON
std::shared_ptr<Layout>      Test_Menu::button_layout        { nullptr };
std::shared_ptr<Button>      Test_Menu::button               { nullptr };

// INPUT FORM
std::shared_ptr<Layout>      Test_Menu::form_layout          { nullptr };
std::shared_ptr<InputField>  Test_Menu::input_field          { nullptr };
std::shared_ptr<Layout>      Test_Menu::input_button_layout  { nullptr };
std::shared_ptr<Button>      Test_Menu::input_button         { nullptr };

std::shared_ptr<Menu>        Test_Menu::menu                 { nullptr };


const std::shared_ptr<Menu> Test_Menu::construct(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler, std::weak_ptr<CustomClient> connection) {
    main_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.1, 0.8, 0.8)}) );
    pannel_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.25, 0.05, 0.5, 0.05), Container(0.25, 0.15, 0.5, 0.10), Container(0.25, 0.30, 0.5, 0.40)}) );

    std::string medium = "This is some medium-length text that is meant to test things out!";
    std::string large = "What follows is a long piece of text, meant to test out the TextBox class' adaptability functionality. Though this does not say much, it contains many words and occupies much space. The sophistic philosophers of Ancient Greece would most certainly be proud. I think this should be enough for the purposes of this text. If this is not the case, I will make sure to edit this so that the functionality is thoroughly tested.";
    std::string font = "resources/fonts/CourierNew.ttf";
    SDL_Color font_color { 255, 255, 255, 255 };
    
    // BUTTON
    button_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) );
    button_layout->placeUI_Element(std::make_shared<TextBox>(context, "Ping Server", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    button = std::make_shared<Button>(context, handler, button_layout, 0, true, false, SDL_Color({172, 43, 12, 125}));
    button->registerCallBack([connection](const GraphicsContext& context, const EventHandler& handler, Button& button) {
        button.setColor(SDL_Colour({12, 172, 43, 125}));
        if(auto c = connection.lock()) {
                if(c->IsConnected())
                    c->PingServer();
            }
    });
    pannel_layout->placeUI_Element(button, 0);

    // pannel_layout->placeUI_Element(new TextBox(context, "Short Text", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    // pannel_layout->placeUI_Element(new TextBox(context, medium, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 1);
    
    // INPUT FORM
    form_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.05, 0.05, 0.2, 0.9), Container(0.35, 0.05, 0.6, 0.9)}) );
    std::shared_ptr<InputField> input_field { std::make_shared<InputField>(context, handler, 0, font, 15, SDL_Color({0, 0, 0, 255})) };
    input_button_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) );
    input_button_layout->placeUI_Element(std::make_shared<TextBox>(context, "Submit username:", font, 15, font_color), 0);
    input_button = std::make_shared<Button>(context, handler, input_button_layout, 0, true, false, SDL_Color({172, 43, 12, 125}));
    std::weak_ptr<InputField> field(input_field);
    input_button->registerCallBack([field, connection](const GraphicsContext& context, const EventHandler& handler, Button& button) mutable {
        if(auto input_field = field.lock()) {
            button.setColor(SDL_Colour({12, 172, 43, 125}));
            std::string input = input_field->getText();
            std::cout << "USER INPUT: " << input << '\n';
            if(auto c = connection.lock()) {
                if(c->IsConnected())
                    c->JoinRandomSession(input);
            }
        }
        else
            std::cout << "Weak pointer could not be resolved!\n";
    });
    form_layout->placeUI_Element(input_button, 0);
    form_layout->placeUI_Element(input_field, 1);
    pannel_layout->placeUI_Element(form_layout, 1);

    pannel_layout->placeUI_Element(std::make_shared<TextBox>(context, large, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 2);
    main_layout->placeUI_Element(std::make_shared<Pannel>(context, pannel_layout, SDL_Color({24, 43, 100, 255})), 0);
    // main_layout->placeUI_Element(new Pannel(context, nullptr, SDL_Color({24, 43, 100, 255})), 0);
    menu = std::make_shared<Menu>(context, handler, main_layout, SDL_Color({255, 255, 255, 255}));
    return menu;
}