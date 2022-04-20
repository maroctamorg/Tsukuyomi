#include "main.hpp"

// sound_effect on button_press + background music
int main() {

    //define UI_Handler
    auto handler = std::make_unique<UI_Handler>(UI::W_W, UI::W_H, "Dynamo Demo");

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
    e_handler->registerKeyCallback(SDLK_TAB, [a_handler, hideshow_a_id](){
        a_handler->start(hideshow_a_id);
        return DEFEvent({EVENT_TYPES::CUSTOM_EVENT, -1, NULL});
    });

    try {
        handler->main();
    } catch(...) {
        std::cerr << "main() encountered an exception... proper exception handling yet to be implemented... exiting...\n";
    }

    return 0;
}