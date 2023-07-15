#include "../QSDL++/Window"

Window::Window(string title, int width, int height, Uint32 flags) : title(title), width(width), height(height), flags(flags)
{
	this->FPS = 120;
	this->frameDelay = 1000 / this->FPS;
	this->running = true;
	this->thereWasPendingEvent = false;
	this->focusedComponent = nullptr;
	// this->argc = 0;
	this->maxFocusTimeID = 0;

	this->x = SDL_WINDOWPOS_CENTERED;
	this->y = SDL_WINDOWPOS_CENTERED;

	SDL_DisplayMode dm;
	ENSURE(SDL_GetCurrentDisplayMode(0, &dm), 0);

	this->baseWindow = nullptr;
	this->mainRenderer = nullptr;
}

void Window::handleEvents()
{
	Event e = Event::toEvent(this->event);

	if (event.type == SDL_QUIT)
	{
		this->running = false;
	}

	else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_GetWindowSize(this->baseWindow, &this->width, &this->height);
		this->onWindowResized(e);
	}
}

void Window::mainloop()
{
	if (SDL_PollEvent(&this->event))
	{
		this->thereWasPendingEvent = true;
		this->handleEvents();
		this->handleFocusing();
	}
	else
	{
		this->thereWasPendingEvent = false;
	}

	ENSURE(SDL_SetRenderDrawColor(this->mainRenderer, WHITE), 0);
	ENSURE(SDL_RenderClear(this->mainRenderer), 0);

	// do here ..

	for (Frame *frame : this->frames)
	{
		this->renderFrame(frame);
		frame->update(); // update the frame before blitting into it.
		this->renderComponent(frame);
	}

	SDL_RenderPresent(this->mainRenderer);
}

void Window::show()
{
	this->baseWindow = SDL_CreateWindow(this->title.c_str(), x, y, width,
										height,
										this->flags);
	ENSURE_NOT(this->baseWindow, NULL);

	this->mainRenderer = SDL_CreateRenderer(this->baseWindow, -1, 0);
	ENSURE_NOT(this->mainRenderer, NULL);

	ENSURE(SDL_SetRenderDrawBlendMode(this->mainRenderer, SDL_BLENDMODE_BLEND), 0);

	while (this->running)
	{
		int frameStart = SDL_GetTicks();

		this->mainloop();

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay)
			SDL_Delay(frameDelay - frameTime);
	}

	SDL_DestroyRenderer(this->mainRenderer);
	SDL_DestroyWindow(this->baseWindow);
}

Point Window::getPosition()
{
	return {this->x, this->y};
}

Dimension Window::getSize()
{
	return {this->width, this->height};
}

Uint16 Window::getFPS()
{
	return this->FPS;
}

string Window::getTitle()
{
	return this->title;
}

Dimension Window::getMaximumSize()
{
	int w, h;
	SDL_GetWindowMaximumSize(this->baseWindow, &w, &h);
	return {w, h};
}

Dimension Window::getMinimumSize()
{
	int w, h;
	SDL_GetWindowMinimumSize(this->baseWindow, &w, &h);
	return {w, h};
}

void Window::setSize(Uint16 width, Uint16 height)
{
	if (this->baseWindow)
		SDL_SetWindowSize(this->baseWindow, width, height);

	this->width = width;
	this->height = height;
}

void Window::setPosition(int x, int y)
{
	if (this->baseWindow)
		SDL_SetWindowPosition(this->baseWindow, x, y);

	this->x = x;
	this->y = y;
}

void Window::setCentered()
{
	this->x = SDL_WINDOWPOS_CENTERED;
	this->y = SDL_WINDOWPOS_CENTERED;
	if (this->baseWindow)
		SDL_SetWindowPosition(this->baseWindow, this->x, this->y);
}

void Window::setFullscreen(bool fscreen)
{
	if (fscreen)
	{
		if (this->baseWindow)
			SDL_SetWindowFullscreen(this->baseWindow, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		if (this->baseWindow)
			SDL_SetWindowFullscreen(this->baseWindow, 0);
	}
}

void Window::setFPS(Uint16 fps)
{
	this->FPS = fps;
	this->frameDelay = 1000 / this->FPS;
}

void Window::setTitle(string title)
{
	if (this->baseWindow)
		SDL_SetWindowTitle(this->baseWindow, title.c_str());
	
	this->title = title;
}

void Window::setMaximized()
{
	if (this->baseWindow)
		SDL_MaximizeWindow(this->baseWindow);
}

void Window::setMinimized()
{
	if (this->baseWindow)
		SDL_MinimizeWindow(this->baseWindow);
}

void Window::setMaximumSize(int width, int height)
{
	if (this->baseWindow)
		SDL_SetWindowMaximumSize(this->baseWindow, width, height);
}

void Window::setMinimumSize(int width, int height)
{
	if (this->baseWindow)
		SDL_SetWindowMinimumSize(this->baseWindow, width, height);
}

void Window::setBordered(bool bordered)
{
	if (this->baseWindow)
		SDL_SetWindowBordered(this->baseWindow, bordered ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE);
}

void Window::setAlwaysOnTop(bool onTop)
{
	if (this->baseWindow)
		SDL_SetWindowAlwaysOnTop(this->baseWindow, onTop ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE);
}

void Window::setResizable(bool resizable)
{
	if (this->baseWindow)
		SDL_SetWindowResizable(this->baseWindow, resizable ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE);
}

void Window::renderComponent(UIComponent *uicomponent)
{
	if (!uicomponent->isVisible())
		return;

	SDL_Texture *texture = SDL_CreateTexture(this->mainRenderer, SDL_PIXELFORMAT_RGBA32, SDL_RENDERER_TARGETTEXTURE, uicomponent->getSize().w, uicomponent->getSize().h);
	ENSURE_NOT(texture, NULL);

	ENSURE(SDL_UpdateTexture(texture, NULL, uicomponent->getSDLSurface()->pixels, uicomponent->getSDLSurface()->pitch), 0);

	int srcx = 0;
	int srcy = 0;
	int srcw = uicomponent->getSize().w;
	int srch = uicomponent->getSize().h;

	int destx = uicomponent->getAbsPosition().x;
	int desty = uicomponent->getAbsPosition().y;
	int destw = srcw;
	int desth = srch;

	Frame *frameComp = dynamic_cast<Frame *>(uicomponent);
	if (frameComp)
	{
		srcx += frameComp->viewport.x;
		srcy += frameComp->viewport.y;
		srcw = frameComp->viewport.w;
		srch = frameComp->viewport.h;

		destw = srcw;
		desth = srch;
	}

	SDL_Rect rsrc = {srcx, srcy, srcw, srch};
	SDL_Rect rdest = {destx, desty, destw, desth};

	ENSURE(SDL_RenderCopy(this->mainRenderer, texture, &rsrc, &rdest), 0);
	SDL_DestroyTexture(texture);

	if (this->thereWasPendingEvent)
		uicomponent->invokeEvents(Event::toEvent(this->event));
}

void Window::renderFrame(Frame *frame)
{
	if (!frame->isVisible())
		return;

	frame->fill(frame->getFillColor());

	// render all frame content
	for (RUIComponent *childComp : frame->getMembers())
	{
		if (!childComp->isVisible())
			continue;

		// Copy component surface into parent frame

		Frame *childFrame = dynamic_cast<Frame *>(childComp);
		if (childFrame)
		{
			this->renderFrame(childFrame);

			childFrame->update();

			int srcx = childFrame->viewport.x;
			int srcy = childFrame->viewport.y;
			int srcw = childFrame->viewport.w;
			int srch = childFrame->viewport.h;

			int destx = childFrame->getAbsPosition().x;
			int desty = childFrame->getAbsPosition().y;
			int destw = srcw;
			int desth = srch;

			SDL_Rect rsrc = {srcx, srcy, srcw, srch};
			SDL_Rect rdest = {destx, desty, destw, desth};

			ENSURE(SDL_BlitSurface(childFrame->getSDLSurface(), &rsrc, frame->getSDLSurface(), &rdest), 0);
		}
		else
		{
			SDL_Rect rdest =
				{childComp->getPosition().x, childComp->getPosition().y,
				 childComp->getSize().w, childComp->getSize().h};

			SDL_Rect rsrc =
				{0, 0, childComp->getSize().w, childComp->getSize().h};
			ENSURE(SDL_BlitSurface(childComp->getSDLSurface(), &rsrc, frame->getSDLSurface(), &rdest), 0);
		}

		if (childComp->hasFocus() && childComp != this->focusedComponent)
		{
			if (childComp->focusTimeID > maxFocusTimeID)
				maxFocusTimeID = childComp->focusTimeID;

			haveFocus.push_back(childComp);
		}

		if (this->thereWasPendingEvent)
		{
			childComp->invokeEvents(Event::toEvent(this->event));
		}
	}

	// for focusing purposes ..
	if (frame->hasFocus() && frame != this->focusedComponent)
	{
		if (frame->focusTimeID > maxFocusTimeID)
			maxFocusTimeID = frame->focusTimeID;

		haveFocus.push_back(frame);
	}
}

void Window::handleFocusing()
{
	for (RUIComponent *comp : haveFocus)
	{
		if (comp->focusTimeID >= maxFocusTimeID)
		{
			if (this->focusedComponent)
				this->focusedComponent->setFocus(false);

			comp->setFocus(true);
			this->focusedComponent = comp;
			break;
		}
	}

	this->haveFocus.clear();
}

void Window::addFrame(Frame *frame)
{
	this->frames.push_back(frame);
}

void Window::popFrame(Frame *frame)
{
	for (int i = 0; i < this->frames.size(); i++)
	{
		if (this->frames.at(i) == frame)
		{
			this->frames.erase(this->frames.begin() + i);
			return;
		}
	}

	printf("WARNING: Cannot delete non existing frame.\n");
}

RUIComponent *Window::getFocusedComponent()
{
	return this->focusedComponent;
}

void Window::close()
{
	this->running = false;
}

Window::~Window()
{
}

// Event handlers

void Window::onWindowResized(Event e)
{
}
