#pragma once

#include "common.hpp"

class Event
{
public:
    Event();
    ~Event();

    Uint32 type;
    Sint32 code;
    EventButton button;
    EventKey key;
    EventWheel wheel;

    static Event toEvent(SDL_Event e);
    static SDL_Event toSDLEvent(Event event);

    void push();
};
