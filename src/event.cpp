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
    Event event;
    event.button.type = e.button.button;
    event.button.x = e.button.x;
    event.button.y = e.button.y;
    event.type = e.type;
    event.code = e.user.code;
    event.key.type = e.key.keysym.sym;
    return event;
}

SDL_Event Event::toSDLEvent(Event event)
{
    SDL_Event e;
    e.button.button = event.button.type;
    e.button.x = event.button.x;
    e.button.y = event.button.y;
    e.type = event.type;
    e.user.code = event.code;
    e.key.keysym.sym = event.key.type;
    return e;
}

Event::~Event()
{
}
