#pragma once

#include "common.hpp"

class Event
{

public:
    Event();
    ~Event();

    Uint32 type;
    EventButton button;
    Uint32 code;

    void push();
};

Event toEvent(SDL_Event e);
