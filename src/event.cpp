#include "../QSDL++/Event"

Event::Event()
{
	this->type = 0;
	this->code = 0;
}

void Event::push()
{
	SDL_Event e = Event::toSDLEvent(*this);
	ENSURE(SDL_PushEvent(&e), 1);
}

Event Event::toEvent(SDL_Event e)
{
	Event event;
	event.button.type = e.button.type;
	event.button.x = e.button.x;
	event.button.y = e.button.y;
	event.type = e.type;
	event.code = e.user.code;
	event.wheel.type = e.wheel.type;
	event.wheel.x = e.wheel.x;
	event.wheel.y = e.wheel.y;
	event.wheel.mouseX = e.wheel.mouseX;
	event.wheel.mouseY = e.wheel.mouseY;
	event.key.type = e.key.keysym.sym;

	return event;
}

SDL_Event Event::toSDLEvent(Event event)
{
	SDL_Event e;
	e.button.type = event.button.type;
	e.button.x = event.button.x;
	e.button.y = event.button.y;
	e.type = event.type;
	e.user.code = event.code;
	e.key.keysym.sym = event.key.type;
	if (event.wheel.type <= 1027)
		e.wheel.type = event.wheel.type;
	e.wheel.x = event.wheel.x;
	e.wheel.y = event.wheel.y;
	e.wheel.mouseX = event.wheel.mouseX;
	e.wheel.mouseY = event.wheel.mouseY;

	return e;
}

Event::~Event()
{
}
