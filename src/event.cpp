#include "../QSDL++/Event"

Event::Event()
{
	this->type = 0;
	this->code = 0;
	this->button.type = 0;
}

void Event::push()
{
	SDL_Event e = this->toSDLEvent();
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

SDL_Event Event::toSDLEvent()
{
	SDL_Event e;
	SDL_zero(e);
	e.type = this->type;
	e.button.type = this->button.type;
	if (this->button.type)
	{
		e.button.x = this->button.x;
		e.button.y = this->button.y;
	}
	e.user.code = this->code;
	e.key.keysym.sym = this->key.type;
	if (this->wheel.type <= 1027)
		e.wheel.type = this->wheel.type;
	e.wheel.x = this->wheel.x;
	e.wheel.y = this->wheel.y;
	e.wheel.mouseX = this->wheel.mouseX;
	e.wheel.mouseY = this->wheel.mouseY;

	return e;
}

Event::~Event()
{
}
