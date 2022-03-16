#include "layout.hpp"

void Container::place(UI_Element* element, const SDL_Rect& rect) {
    // ui_element = std::move(element);
    std::cout << "updating element position\n";
    SDL_Rect position { static_cast<int>(r_x*rect.w + rect.x), static_cast<int>(r_y*rect.h + rect.y), static_cast<int>(r_w*rect.w), static_cast<int>(r_h*rect.h) };
    element->updatePosition(position);
}

// void Layout::placeUI_Element(std::unique_ptr<UI_Element> element, int index) {
//     int length = ui_elements.size();
//     if(index >= containers.size()) {
//         std::cout << "!!!!!!!!!\tAttempted to placeUI_Element on a non-existent container!\t!!!!!!!!!\n";
//         return;
//     }
//     else if(index < 0 && length < containers.size()) {
//         ui_elements.push_back(std::move(element));
//         containers.at(length).place(ui_elements.at(length).get(), this->rect);
//         return;
//     }
//     else if(index >= length)
//         ui_elements.resize(index + 1);
//     ui_elements.at(index) = std::move(element);
//     containers.at(index).place(ui_elements.at(index).get(), this->rect);
// }

void Layout::placeUI_Element(std::shared_ptr<UI_Element> element, int index) {
    int length = ui_elements.size();
    if(index >= containers.size()) {
        std::cout << "!!!!!!!!!\tAttempted to placeUI_Element on a non-existent container!\t!!!!!!!!!\n";
        return;
    }
    else if(index < 0 && length < containers.size()) {
        ui_elements.push_back(element);
        containers.at(length).place(ui_elements.at(length).get(), this->rect);
        return;
    }
    else if(index >= length)
        ui_elements.resize(index + 1);
    ui_elements.at(index) = element;
    containers.at(index).place(ui_elements.at(index).get(), this->rect);
}

void Layout::placeUI_Element(UI_Element* a_element, int index) {
    std::shared_ptr<UI_Element> element{a_element};
    int length = ui_elements.size();
    if(index >= containers.size()) {
        std::cout << "!!!!!!!!!\tAttempted to placeUI_Element on a non-existent container!\t!!!!!!!!!\n";
        return;
    }
    else if(index < 0 && length < containers.size()) {
        ui_elements.push_back(element);
        containers.at(length).place(ui_elements.at(length).get(), this->rect);
        return;
    }
    else if(index >= length)
        ui_elements.resize(index + 1);
    ui_elements.at(index) = element;
    containers.at(index).place(ui_elements.at(index).get(), this->rect);
}

std::shared_ptr<UI_Element> Layout::getUIElement(int index) {
    if(index < 0 || index >= ui_elements.size()) return nullptr;
    return ui_elements.at(index);
    
}
Container Layout::getContainer(int index) {
    if(index < 0 || index >= containers.size()) return Container(0, 0, 0, 0);
    return containers.at(index);
}

// should be separated into addContainer, removeContainer and updateContainer
void Layout::addContainer(int index, Container container) {
    if(index < 0) {
        std::cout << "Invalid index passed to addContainer call in Layout object...\n";
        return;
    }
    else if(index >= containers.size()) {
        containers.resize(index+1);
        containers.at(index) = container;
    }
    else {
        std::cout << "placing into new container\n";
        containers.at(index) = container;
        if(ui_elements.at(index).get()) containers.at(index).place(ui_elements.at(index).get(), this->rect); // this should not be done here, and rather in Layout::update to guarantee thread safety
    }
}

void Layout::render() {
    // std::cout << "##############\tCall to render Layout!\t##############";
    this->UI_Element::render();
    for(int i = 0; i < ui_elements.size(); i++)
        if(ui_elements.at(i).get() && !ui_elements.at(i)->getHidden()) ui_elements.at(i)->render();
}
void Layout::update() {
    for(int i = 0; i < ui_elements.size(); i++)
        if(ui_elements.at(i).get()) ui_elements.at(i)->update();
    this->updateSize();
}
void Layout::updateSize() {
    // std::cout << "Call to update layout size...\n";
    if(ui_elements.size() < containers.size()) {
        std::cout << "Fewer elements than containers in call to Layout::updateSize!\n";
        // return;
    }
    if(containers.size() < ui_elements.size()) {
        std::cout << "Fewer containers than elements in call to Layout::updateSize()\n";
        return;
    }
    for (int i = 0; i < ui_elements.size(); i++)
        if(ui_elements.at(i).get()) containers.at(i).place(ui_elements.at(i).get(), this->rect);
}
void Layout::updatePosition(const SDL_Rect& rect) {
    this->UI_Element::updatePosition(rect);
    this->updateSize();
}