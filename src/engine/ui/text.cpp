#include "text.hpp"

std::string TextBox::getText(){
    return this->contents;
}

void TextBox::getCharTextureSize(int* w, int* h){
    if(this->lines.size() == 0) {
        Text dummyText {context->renderer,"abcdefghijklmnopqrstuvwxyz", font, ptsize, color};
        dummyText.getCharacterTextureSize(w, h);
        return;
    }
    this->lines.at(0)->getCharacterTextureSize(w, h);
    // if(*w == 0 || *h == 0) {
    //     Text dummyText {context->renderer,"abcdefghijklmnopqrstuvwxyz", font, ptsize, color};
    //     dummyText.getCharacterTextureSize(w, h);
    //     return;
    // }
}

int TextBox::numberOfLines() {
    return this->lines.size();
}

SDL_Point TextBox::getPos(int line, int charPos) {
    SDL_Point initialPos;
    int x,w,h;
    if (line == 0 && this->lines.size() == 0) {
        Text dummyText {context->renderer,"abcdefghijklmnopqrstuvwxyz", font, ptsize, color};
        initialPos = dummyText.getPos(context->renderer, this->rect, this->align_x, this->align_y);
        dummyText.getCharacterTextureSize(&w, &h);
        x = initialPos.x + w*charPos;
    }
    else if(line < 0 || line >= this->lines.size()) return {-100, -100};
    else {
        initialPos = this->lines.at(line)->getPos(context->renderer, this->rect, this->align_x, this->align_y);
        this->getCharTextureSize(&w, &h);
        x = initialPos.x + w*charPos;
    }
    return {x, initialPos.y};
}

void TextBox::updateAlignment(ALIGN_X alignX, ALIGN_Y alignY) {
    this->align_x = alignX;
    this->align_y = alignY;
}

void TextBox::updateFontSize(int ptsize) {
    this->ptsize = ptsize;
    for(int i {0}; i < this->lines.size(); i++)
        this->lines.at(i)->updateFontSize(this->context->renderer, this->ptsize);
}

int TextBox::calculateCapacity() {
    int w, h;
    this->getCharTextureSize(&w, &h);
    int capacity = static_cast<int>(this->rect.h/h - 0.5)*static_cast<int>(this->rect.w/w - 0.5);
    // std::cout << "Returning from calculateCapacity with w=" << w << "\th=" << h << "\tcapacity: " << capacity << '\n';
    return capacity;
}

bool TextBox::checkOverflow() {
    int w, h;
    // check vertical overflow
    Text text { context->renderer, contents, font, ptsize, font_color };
    text.getCharacterTextureSize(&w, &h);
    if(this->lines.size() * h > 9*this->rect.h/10)
        return true;
    // check width overflow for each line
    for(int i {0}; i < this->lines.size(); i++) {
        this->lines.at(i)->getCharacterTextureSize(&w, &h);
        if(this->lines.at(i)->getLength() * w > 9*this->rect.w/10)
            return true;
    }
    return false;
}

// returns number of characters stored in strings for lines of text (both start and end indices included)
int TextBox::getContentLengthFromLines(int start_index, int end_index) {
    int total {0};
    if(end_index < 0 || end_index >= this->lines.size())
        end_index = this->lines.size() - 1;
    for(int i {start_index}; i <= end_index; i++)
        total += this->lines.at(i)->getLength();
    return total;
}

// RECURSIVE-ISH DEFINITION
bool TextBox::breakContentsToLines(int start_index, int content_pointer) {
    // std::cout << "############\tCall to breakContentsToLines\t############\n";
    if(this->rect.w == 0 || this->rect.h == 0 || start_index > this->lines.size() || (start_index != 0 && start_index == this->lines.size()))
        return false;
    if(this->contents.length() < 1) {
        this->lines.clear();
        return true;
    }
    if(content_pointer >= this->contents.length())
        return true;
    int new_content_pointer { content_pointer };
    if(this->lines.size() == 0) {
        this->lines.push_back(std::make_unique<Text>(context->renderer, this->contents, font, ptsize, font_color));
        content_pointer = 0;
    } else {
        for(int i{start_index}; this->lines.size() > start_index; i++) {
            this->lines.pop_back();
        }
        // the content-pointer points to the character in the content string corresponding to the current string
        this->lines.push_back(std::make_unique<Text>(context->renderer, this->contents.substr(content_pointer, this->contents.length() - content_pointer), font, ptsize, font_color));
    }    
    // CHECK IF TEXTURE HAS BEEN SUCCESSFULLY LOADED!
    if(!this->lines.at(start_index)->isLoaded()) {
        std::cout << "!!!!!\tText texture is not loaded: call to TextBox::breakContentsToLine()\t!!!!!\n";
        return false;
    }
    int length = contents.length();
    int w, h;

    // go through indexes of each line recursively, untill a new-line character is found or overflow occurs
    // in such case break the line
    // std::cout << "###########\tSTARTING RECURSIVE BREAKLINE CHECK\t###########\n";
    this->lines.at(start_index)->getCharacterTextureSize(&w, &h);
    std::string lcontent = this->lines.at(start_index)->getString();
    length = lcontent.length();
    int brk_delim = 9*this->rect.w/(w*10);
    for(int index {0}; index < brk_delim && index < length; index++) {
        // does it get saved as a single character? If not, then the implementation must be tweaked slightly
        if(lcontent.at(index) == '\n') {
            // breakLine(start_index, index);
            std::array<std::string, 2> broken_text { this->lines.at(start_index)->split(index) };
            if(broken_text.at(1).length() == 0) {
                break;
            } else {
                new_content_pointer += index;
                // the substring call below serves to remove the newLine char
                this->lines.at(start_index) = std::make_unique<Text>(context->renderer, broken_text.at(0).substr(0, index), font, ptsize, font_color);
                this->lines.push_back(std::make_unique<Text>(context->renderer, broken_text.at(1), font, ptsize, font_color));
                if(start_index < this->lines.size() - 1)
                    return this->breakContentsToLines(start_index + 1, new_content_pointer);
            }
        }
    }
    //if it returns from here (no newlines), check overflow!
    if( const double diff = length - brk_delim; diff >= 1 ) {
        // find closest whitespace before the break-point and break
        int index = length - static_cast<int>(diff);
        bool done {false};
        while(index > 0 && !done) {
            if(lcontent.at(index) == ' ') {
                // std::cout << "Breaking line at index: " << index << "\n";
                std::array<std::string, 2> broken_text { this->lines.at(start_index)->split(index) };
                if(broken_text.at(1).length() == 0) {
                    continue;
                } else {
                    new_content_pointer += index;
                    // std::cout << "Returned: " << broken_text.at(0) << "\t" << broken_text.at(1) << "\n";
                    this->lines.at(start_index) = std::make_unique<Text>(context->renderer, broken_text.at(0), font, ptsize, font_color);
                    this->lines.push_back(std::make_unique<Text>(context->renderer, broken_text.at(1), font, ptsize, font_color));
                    return this->breakContentsToLines(start_index + 1, new_content_pointer);
                }
            }
            index--;
        }
    }
    // once it has gone through all of the above it can return true safely
    return true;
}

// PURELY ITERATIVE DEFINITION
// bool TextBox::breakContentsToLines() {
//     // std::cout << "############\tCall to breakContentsToLines\t############\n";
//     if(this->rect.w == 0 || this->rect.h == 0)
//         return false;
//     if(this->lines.size() != 1) {
//         this->lines = std::vector<std::unique_ptr<Text>>(0);
//         this->lines.push_back(std::make_unique<Text>(context->renderer, contents, font, ptsize, font_color));
//     }
//     // CHECK IF TEXTURE HAS BEEN SUCCESSFULLY LOADED!
//     if(!this->lines.at(0)->isLoaded()) {
//         std::cout << "!!!!!\tText texture is not loaded: call to TextBox::adaptContentsToBox()\t!!!!!\n";
//         return false;
//     }
//     int length = contents.length();
//     int i {0};
//     int w, h;
//     while(true) {
//         // check for width overflow line-by-line
//         if(!this->lines.at(i)->isLoaded()) {
//             std::cout << "!!!!!\tText texture being checked in TextBox::adaptContentsToBox() is not loaded!\t!!!!!";
//             return false;
//         }
//         this->lines.at(i)->getCharacterTextureSize(&w, &h);
//         length = this->lines.at(i)->getLength();
//         if( const double diff = length*w - 9*this->rect.w/10; diff >= 1 ) {
//             std::string lcontents = this->lines.at(i)->getString();
//             int index = length - static_cast<int>(diff/w);
//             bool done {false};
//             for(int j{0}; j < index && !done; j++) {
//                 if(lcontents.at(j) == '\n') {
//                     std::array<std::string, 2> broken_text { this->lines.at(i)->split(index) };
//                     if(broken_text.at(1).length() == 0) {
//                         done = true;
//                     } else {
//                         this->lines.at(i) = std::make_unique<Text>(context->renderer, broken_text.at(0).substr(0, j), font, ptsize, font_color);
//                         this->lines.push_back(std::make_unique<Text>(context->renderer, broken_text.at(1), font, ptsize, font_color));
//                         done = true;
//                     }
//                 }
//             }
//             // find closest whitespace before the break-point
//             while(index > 0 && !done) {
//                 if(lcontents.at(index) == ' ') {
//                     // std::cout << "Breaking line at index: " << index << "\n";
//                     std::array<std::string, 2> broken_text { this->lines.at(i)->split(index) };
//                     // std::cout << "Returned: " << broken_text.at(0) << "\t" << broken_text.at(1) << "\n";
//                     if(broken_text.at(1).length() == 0) {
//                         done = true;
//                     } else {
//                         this->lines.at(i) = std::make_unique<Text>(context->renderer, broken_text.at(0), font, ptsize, font_color);
//                         this->lines.push_back(std::make_unique<Text>(context->renderer, broken_text.at(1), font, ptsize, font_color));
//                         done = true;
//                     }
//                 }
//                 index--;
//             }
//         }
//         i++;
//         if(i >= this->lines.size()) {
//             // std::cout << "############\tReturning form breakContentsToLines with " << this->lines.size() << " lines\t############\n";
//             return true;
//         }
//     }
// }


void TextBox::adaptContentsToBox() {
    // !!! MUST RETHINK IMPLEMENTATION, FOR NOW IT IS NO LONGER BEING USED!

    // if(!this->breakContentsToLines())
    //     return;
    // std::cout << "############\tCall to adaptContentsToBox\t############\n";
    // bool repeat {false};
    // // if(this->rect.w == 0 || this->rect.h == 0)
    // //     return;
    // // if(this->lines.size() != 1) {
    // //     this->lines = std::vector<std::unique_ptr<Text>>(0);
    // //     this->lines.push_back(std::make_unique<Text>(context->renderer, contents, font, ptsize, font_color));
    // // }
    // // // CHECK IF TEXTURE HAS BEEN SUCCESSFULLY LOADED!
    // // if(!this->lines.at(0)->isLoaded()) {
    // //     std::cout << "!!!!!\tText texture is not loaded: call to TextBox::adaptContentsToBox()\t!!!!!\n";
    // //     return;
    // // }
    // int w, h;
    // this->lines.at(0)->getCharacterTextureSize(&w, &h);
    // int length = contents.length();

    // // if much larger, decrease point size
    // // std::cout << "Checking txt_texture size!\nrect.w: " << rect.w <<"\trect.h: " << rect.h << "\nTexture size:\tw: " << w << "\th: " << h << "\nLength: " << length << "\n";
    // double quotient = length*w / this->rect.w;
    // if( quotient*h > this->rect.h) {
    //     // std::cout << "############\tDecreasing point size to " << this->lines.at(0)->getPtSize() - 2 << "!\t############\n";
    //     this->updateFontSize(this->lines.at(0)->getPtSize() - 2);
    //     // this->adaptContentsToBox();
    //     repeat = true;
    // }
    // // if much smaller, increase point size
    // else if (quotient*h < this->rect.w/3) {
    //     // std::cout << "############\tIncreasing point size!\t############\n";
    //     this->updateFontSize(this->lines.at(0)->getPtSize() + 2);
    //     // this->adaptContentsToBox();
    //     repeat = true;
    // }
    // // std::cout << "############\tLeaving adaptContentsToBox...\t############\n";
    // if(repeat)
    //     this->adaptContentsToBox();
}
void TextBox::append(char a) {
    // std::cout << "Call to TextBox::append with char " << a << "!\n";
    this->contents.push_back(a);
    this->breakContentsToLines();
    // this->breakContentsToLines(this->lines.size() - 1, contents.length() - 1); // IS IT -1 OR JUST .LENGTH()?
}
bool TextBox::del() {
    if(this->contents.size() < 1)
        return false;
    else if(this->contents.size() == 1)
        this->contents.clear();
    else
        this->contents.pop_back();
    this->breakContentsToLines();
    return true;
    // const int length1 = this->lines.at(this->lines.size() - 2)->getLength();
    // const int length2 = this->lines.back()->getLength();
    // if(length2 <= 1)
    //     this->breakContentsToLines(this->lines.size() - 2, this->contents.size() - 1 - (length1 + length2));
    // else
    //     this->breakContentsToLines(this->lines.size() - 1, this->contents.size() - 1 - length2);
}
void TextBox::updateText(std::string text) {
    this->contents = text;
    this->breakContentsToLines();
}
void TextBox::render() {
    // std::cout << "###############\tCall to render text_box!\t###############\n";
    this->UI_Element::render();
    SDL_Rect target = this->rect;
    int w, h;
    for(int i {0}; i < this->lines.size(); i++) {
        if(!this->lines.at(i)->isLoaded()) {
            std::cout << "!!!!!\tText texture being rendered is not loaded!\t!!!!!\n";
            return;
        }
        this->lines.at(i)->getCharacterTextureSize(&w, &h);
        // std::cout << "Target Rect: {" << target.x << ", " << target.y << ", " << target.w << ", " << target.h << "}\n";
        this->lines.at(i)->display(context->renderer, target, this->align_x, this->align_y);
        target.y += 2*h;
    }
    return;
}
// void TextBox::update() {
    
// }
void TextBox::updateSize() {
    // std::cout << "Call to TextBox::updateSize()!\n";
    if(this->checkOverflow())
        this->breakContentsToLines();
        // this->adaptContentsToBox();
}
void TextBox::updatePosition (const SDL_Rect& rect) {
    // std::cout << "Call to TextBox::updatePosition()!\n";
    this->UI_Element::updatePosition(rect);
    this->breakContentsToLines();
    // this->adaptContentsToBox();
}