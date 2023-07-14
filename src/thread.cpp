#include "../QSDL++/Thread"

Thread::Thread(string name, void *data) : name(name), data(data)
{
	this->fnPtr = nullptr;
	this->baseThread = nullptr;
}

void Thread::setName(string name)
{
	this->name = name;
}

void Thread::setData(void *data)
{
	this->data = data;
}

void* Thread::getData()
{
	return this->data;
}

string Thread::getName()
{
	return this->name;
}

int Thread::run(void *data)
{
	return 0;
}

void Thread::start()
{
	if (this->fnPtr)
	{
		this->baseThread = SDL_CreateThread(this->fnPtr, this->name.c_str(),
				this->data);
		ENSURE_NOT(this->baseThread, NULL);
	}
	else
	{
		this->baseThread = SDL_CreateThread(&Thread::run, this->name.c_str(),
				this->data);
		ENSURE_NOT(this->baseThread, NULL);
	}
}

int Thread::join()
{
	int __returned;
	SDL_WaitThread(this->baseThread, &__returned);
	return __returned;
}

void Thread::setTarget(int (*fnPtr)(void*))
{
	this->fnPtr = fnPtr;
}

Thread::~Thread()
{
}
