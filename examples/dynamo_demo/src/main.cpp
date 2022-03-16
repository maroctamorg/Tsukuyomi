#include "main.hpp"

// sound_effect on button_press + background music
int main() {

    //define UI_Handler
    auto handler = std::make_unique<UI_Handler<Dynamics, Overlay>>(UI::W_W, UI::W_H, "Dynamo Demo");

    std::cout << "Defined UI_Handler...\n";

    //get default context and handlers
    std::shared_ptr<Graphics_Context> g_context = handler->getGraphicsContext();
    std::shared_ptr<Animation_Handler> a_handler = handler->getAnimationHandler();
    std::shared_ptr<Event_Handler> e_handler = handler->getEventHandler();

    std::cout << "Obtained default context and handlers...\n";

    //construct Main and Overlay objects
    std::shared_ptr<Dynamics> dynamics = std::make_shared<Dynamics>(g_context);
    std::shared_ptr<Overlay> overlay = std::make_shared<Overlay>(g_context, a_handler, e_handler->main_input_handler,dynamics);

    //setup Main and Overlay
    std::cout << "Setting main and overlay...\n";
    handler->setMain(dynamics);
    handler->setOverlay(overlay);

    //setup event-handling and callbacks
    std::cout << "Setting up event-handling and callbacks...\n";
    uint hideshow_a_id = a_handler->add(std::make_unique<Overlay_HideShow_Animation>(overlay, handler->getLayout(), 1));
    std::weak_ptr<Overlay> ptr_overlay {overlay};
    e_handler->registerKeyCallback([ptr_overlay, a_handler, hideshow_a_id](SDL_Keycode keycode){
        if(keycode == SDLK_TAB) a_handler->start(hideshow_a_id);
    });

    try {
        handler->main();
    } catch(...) {
        std::cerr << "main() encountered an exception... proper exception handling yet to be implemented... exiting...\n";
    }

    return 0;
}