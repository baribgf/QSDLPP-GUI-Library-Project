#pragma once

#include "common.hpp"

class Event
{

public:
    Event();
    ~Event();

    Uint32 type;
    Uint32 code;
    EventButton button;
    EventKey key;

    static Event toEvent(SDL_Event e);
    static SDL_Event toSDLEvent(Event event);

    void push();
};
