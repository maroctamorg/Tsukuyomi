#include "test_menu.hpp"

const std::unique_ptr<Menu> Test_Menu(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler) {
    Layout* main_layout = new Layout(context, {Container(0.1, 0.1, 0.8, 0.8)});
    Layout* pannel_layout = new Layout(context, {Container(0.25, 0.05, 0.5, 0.05), Container(0.25, 0.15, 0.5, 0.10), Container(0.25, 0.30, 0.5, 0.40)});

    std::string medium = "This is some medium-length text that is meant to test things out!";
    std::string large = "What follows is a long piece of text, meant to test out the TextBox class' adaptability functionality. Though this does not say much, it contains many words and occupies much space. The sophistic philosophers of Ancient Greece would most certainly be proud. I think this should be enough for the purposes of this text. If this is not the case, I will make sure to edit this so that the functionality is thoroughly tested.";
    std::string font = "resources/fonts/CourierNew.ttf";
    SDL_Color font_color { 255, 255, 255, 255 };
    
    // BUTTON
    Layout* button_layout = new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)});
    button_layout->placeUI_Element(new TextBox(context, "Button Text", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    Button* button = new Button(context, handler, button_layout, 0, true, false, SDL_Color({172, 43, 12, 125}));
    button->registerCallBack([](GraphicsContext* context, EventHandler* handler, Button* button) {
        button->setColor(SDL_Colour({12, 172, 43, 125}));
    });
    pannel_layout->placeUI_Element(button, 0);

    // pannel_layout->placeUI_Element(new TextBox(context, "Short Text", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    // pannel_layout->placeUI_Element(new TextBox(context, medium, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 1);
    
    // INPUT FORM
    Layout* form_layout = new Layout(context, {Container(0.05, 0.05, 0.2, 0.9), Container(0.35, 0.05, 0.6, 0.9)});
    std::shared_ptr<InputField> input_field { std::make_shared<InputField>(context, handler, 0, font, 15, SDL_Color({0, 0, 0, 255})) };
    Layout* input_button_layout = new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)});
    input_button_layout->placeUI_Element(new TextBox(context, "Submit", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    Button* input_button = new Button(context, handler, input_button_layout, 0, true, false, SDL_Color({172, 43, 12, 125}));
    std::weak_ptr<InputField> field(input_field);
    input_button->registerCallBack([field](GraphicsContext* context, EventHandler* handler, Button* button) {
        if(auto input_field = field.lock())
            std::cout << "USER INPUT: " << input_field->getText() << '\n';
        else
            std::cout << "Weak pointer could not be resolved!\n";
    });
    form_layout->placeUI_Element(input_button, 0);
    form_layout->placeUI_Element(input_field, 1);
    pannel_layout->placeUI_Element(form_layout, 1);

    pannel_layout->placeUI_Element(new TextBox(context, large, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 2);
    main_layout->placeUI_Element(new Pannel(context, pannel_layout, SDL_Color({24, 43, 100, 255})), 0);
    // main_layout->placeUI_Element(new Pannel(context, nullptr, SDL_Color({24, 43, 100, 255})), 0);
    return std::make_unique<Menu>(context, handler, main_layout, SDL_Color({255, 255, 255, 255}));
}