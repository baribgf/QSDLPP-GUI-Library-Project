#pragma once

#include "common.hpp"

class Thread
{
protected:
	string name;
	void *data;
	int (*fnPtr)(void *);
	static int run(void *data);

public:
	SDL_Thread *baseThread;
	
	Thread(string name, void *data);
	~Thread();

	void setName(string name);
	void setData(void *data);

	string getName();
	void *getData();

	void start();
	int join();

	void setTarget(int (*fnPtr)(void *));
};
