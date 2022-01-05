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


    // std::shared_ptr<Graphics_Context> g_context;
    // std::shared_ptr<Animation_Handler> a_handler;
    // std::shared_ptr<Event_Handler> e_handler;

    // do {
    //     std::cout << "Locking graphics context...";
    //     g_context = graphics_context.lock();
    // } while(!(g_context.get()));
    // std::shared_ptr<Dynamics> dynamics = std::make_shared<Dynamics>(g_context);

    // do {
    //     std::cout << "Locking animation handler...";
    //     a_handler = animation_handler.lock();
    // } while(!a_handler);

    // do {
    //     std::cout << "Locking event handler...";
    //     e_handler =  event_handler.lock();
    // } while(!e_handler);
    // std::shared_ptr<Overlay> overlay = std::make_shared<Overlay>(g_context, animation_handler, e_handler->main_input_handler,(*dynamics.get()));

    //setup Main and Overlay
    std::cout << "Setting main and overlay...\n";
    handler->setMain(dynamics);
    handler->setOverlay(overlay);

    //setup event-handling and callbacks
    std::cout << "Setting up event-handling and callbacks...\n";
    e_handler->registerKeyCallback([overlay, a_handler](SDL_Keycode keycode){
        if(keycode == SDLK_SPACE) overlay->setHidden(!overlay->getHidden());
        a_handler->add(std::make_unique<Overlay_Animation>(overlay));
    });

    handler->main();

    return 0;
}