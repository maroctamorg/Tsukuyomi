#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP

#include "extern_ui.hpp"

class Animation {
protected:
    uint counter {0};
    bool active {false};
    bool done {false};

public:
    const bool blocking {false};
    const bool permanent {false};

public:
    bool isActive() const;
    bool isDone() const;
    virtual void start();
    virtual void next();
    void end();
    void reset();

public:
    Animation(bool blocking, bool permanent)
        : blocking {blocking}, permanent {permanent} {}

    virtual ~Animation() = default;
};

// Implement queue for animations that cannot run simultaneously to other animations (all blocking animations?)
// run first queued animations in order, then free animations simultaneously; or always have the free animations running
// and only run queued animations in order in parallel (probably this is the best idea)

class Animation_Handler final {
private:
    uint counter {0};
    std::map<uint, std::unique_ptr<Animation>> animations;

public:
    uint add(std::unique_ptr<Animation> animation);
    void remove(uint animation);
    void start(uint animation);
    bool pending() const; //blocking animations running?
    void update();

public:
    Animation_Handler() = default;
};

#endif