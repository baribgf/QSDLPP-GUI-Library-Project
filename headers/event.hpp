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

    enum EventType
    {
        MOUSE_DOWN    = SDL_MOUSEBUTTONDOWN,
        MOUSE_UP      = SDL_MOUSEBUTTONDOWN,
        MOUSE_MOTION  = SDL_MOUSEMOTION,
        KEY_DOWN      = SDL_KEYDOWN,
        KEY_UP        = SDL_KEYUP,
        WINDOW_RESIZE = SDL_WINDOWEVENT_RESIZED,
        QUIT          = SDL_QUIT,
    };

    enum EventButtonType
    {
        LEFT_BUTTON   = SDL_BUTTON_LEFT,
        RIGHT_BUTTON  = SDL_BUTTON_RIGHT,
        MIDDLE_BUTTON = SDL_BUTTON_MIDDLE
    };

    static Event toEvent(SDL_Event e);
    static SDL_Event toSDLEvent(Event event);

    void push();
};
