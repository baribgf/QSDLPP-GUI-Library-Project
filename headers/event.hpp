#pragma once

#include "common.hpp"

class Event
{
public:
    Event();
    ~Event();

    Uint64 type;
    Uint64 code;
    EventButton button;
    EventKey key;

    static Event toEvent(SDL_Event e);
    static SDL_Event toSDLEvent(Event event);

    void push();
};
