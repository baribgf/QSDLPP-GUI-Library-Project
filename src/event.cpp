#include "../headers/event.hpp"

Event::Event()
{
}

void Event::push()
{
    SDL_Event e = Event::toSDLEvent(*this);
    SDL_PushEvent(&e);
}

Event Event::toEvent(SDL_Event e)
{
    Event ev;
    ev.button.type = e.button.button;
    ev.button.x = e.button.x;
    ev.button.y = e.button.y;
    ev.type = e.type;
    ev.code = e.user.code;
    return ev;
}

SDL_Event Event::toSDLEvent(Event event)
{
    SDL_Event e;
    e.button.button = event.button.type;
    e.button.x = event.button.x;
    e.button.y = event.button.y;
    e.type = event.type;
    e.user.code = event.code;
    return e;
}

Event::~Event()
{
}
