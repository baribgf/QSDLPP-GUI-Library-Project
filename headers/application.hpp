#pragma once

#include "common.hpp"
#include "window.hpp"
#include "thread.hpp"

class Application
{
protected:
	Window* window;

public:
	int argc;
	char const *argv[];

	Application(int argc, char const *argv[]);
	~Application();

	void setWindow(Window *win);

	void exec();
};
