#include "animations.hpp"

bool Animation::isDone() const {
    return done;
}

bool Animation::isActive() const {
    return active;
}

void Animation::start() {
    active = true;
}

void Animation::next() {
    std::cout << "Call to Animation base class next...\n";
    counter++;
}

void Animation::end() {
    // std::cout << "Call to end animation...\n";
    active = false;
    done = true;
}

void Animation::reset() {
    active = false;
    done = false;
}

uint Animation_Handler::add(std::unique_ptr<Animation> animation) {
    counter++;
    animations.insert({counter, std::move(animation)});
    return counter;
}

void Animation_Handler::remove(uint animation) {
    animations.erase(animation);
}

void Animation_Handler::start(uint animation) {
    if(!animations.at(animation)->isActive()) animations.at(animation)->start();
}

bool Animation_Handler::pending() const {
    bool blocking {false};
    std::map<uint, std::unique_ptr<Animation>>::const_iterator it {animations.cbegin()};
    while(it != animations.cend()) {
        blocking = blocking || (it->second->blocking && it->second->isActive());
        it++;
    }
    return blocking;
}

void Animation_Handler::update() {
    // std::cout << "call to update...\n";
    std::map<uint, std::unique_ptr<Animation>>::iterator it {animations.begin()};
    int del {0};
    while(it != animations.end()) {
        // std::cout << "prepare for doom...\n";
        if(it->second->isActive()) it->second->next();
        else if (it->second->isDone()) {
            if(!it->second->permanent) del = it->first;
            else it->second->reset();
        }
        it++;
        if(del > 0) animations.erase(del); // can I erase from inside the iterator loop?
    }
}