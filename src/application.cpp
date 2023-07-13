#include "../headers/application.hpp"

Application::Application(int argc, char const *argv[])
{
	this->argc = argc;
	for (int i = 0; i < argc; i++)
		this->argv[i] = argv[i];

	ENSURE(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS), 0);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	ENSURE(TTF_Init(), 0);
}

void Application::setWindow(Window *win)
{
	win->argc = this->argc;

	for (int i = 0; i < this->argc; i++)
	{
		win->argv[i] = this->argv[i];
	}

	this->window = win;
}

void Application::exec()
{
	this->window->exec();
}

Application::~Application()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
