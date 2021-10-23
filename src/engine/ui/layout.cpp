#include "layout.hpp"

void Container::place(UI_Element* element, const SDL_Rect& rect) {
    // ui_element = std::move(element);
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

void Layout::render() {
    // std::cout << "##############\tCall to render Layout!\t##############";
    this->UI_Element::render();
    for(int i = 0; i < ui_elements.size(); i++)
        ui_elements.at(i)->render();
}
void Layout::update() {
    for(int i = 0; i < ui_elements.size(); i++)
        ui_elements.at(i)->update();
}
void Layout::updateSize() {
    if(ui_elements.size() < containers.size()) {
        std::cout << "Fewer elements than containers in call to Layout::updateSize!\n";
        return;
    }
    for (int i = 0; i < containers.size(); i++)
        containers.at(i).place(ui_elements.at(i).get(), this->rect);
}
void Layout::updatePosition(const SDL_Rect& rect) {
    this->UI_Element::updatePosition(rect);
    this->updateSize();
}