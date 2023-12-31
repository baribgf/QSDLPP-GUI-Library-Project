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
	this->focusChanged = false;

	this->maximized = (SDL_bool)(flags & SDL_WINDOW_MAXIMIZED) == SDL_bool::SDL_TRUE ? true : false;
	this->minimized = (SDL_bool)(flags & SDL_WINDOW_MINIMIZED) == SDL_bool::SDL_TRUE ? true : false;
	this->bordered = (SDL_bool)(flags & SDL_WINDOW_BORDERLESS) == SDL_bool::SDL_FALSE ? true : false;
	this->alwaysOnTop = (SDL_bool)(flags & SDL_WINDOW_ALWAYS_ON_TOP) == SDL_bool::SDL_TRUE ? true : false;
	this->resizable = (SDL_bool)(flags & SDL_WINDOW_RESIZABLE) == SDL_bool::SDL_TRUE ? true : false;

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
		SDL_GetWindowSize(this->baseWindow, &this->width, &this->height); // update window size
		this->onWindowResized(e);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		this->onMousePressed(e);
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		this->onMouseReleased(e);
	}
	else if (event.type == SDL_KEYDOWN)
	{
		this->onKeyPressed(e);
	}
	else if (event.type == SDL_KEYUP)
	{
		this->onKeyReleased(e);
	}
}

void Window::mainloop()
{
	if (this->isMinimized())
		return;
		
	int pollEvent = SDL_PollEvent(&this->event);
	if (pollEvent == 1 || this->focusChanged)
	{
		this->thereWasPendingEvent = true;
		this->focusChanged = false;
		this->handleEvents();

		ENSURE(SDL_SetRenderDrawColor(this->mainRenderer, WHITE), 0);
		ENSURE(SDL_RenderClear(this->mainRenderer), 0);

		// do here ..
		for (int i = 0; i < this->framesQueue.size(); i++)
		{
			this->frames.push_back(this->framesQueue.front());
			this->framesQueue.pop();
		}

		for (Frame *frame : this->frames)
		{
			this->renderFrame(frame);
			frame->update(); // update the frame before blitting into it.
			this->renderComponent(frame);
		}

		SDL_RenderPresent(this->mainRenderer);

		this->handleFocusing();
	}
	else if (pollEvent != 1)
	{
		this->thereWasPendingEvent = false;
	}
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

	SDL_GetWindowPosition(this->baseWindow, &this->x, &this->y);

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

vector<Frame *> Window::getFrames()
{
	return this->frames;
}

bool Window::isMaximized()
{
	return this->maximized;
}

bool Window::isMinimized()
{
	return this->minimized;
}

bool Window::isBordered()
{
	return this->bordered;
}

bool Window::isAlwaysOnTop()
{
	return this->alwaysOnTop;
}

bool Window::isResizable()
{
	return this->resizable;
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

void Window::setMaximized(bool maximized)
{
	if (this->baseWindow)
	{
		if (maximized)
			SDL_MaximizeWindow(this->baseWindow);
		else
			SDL_RestoreWindow(this->baseWindow);
	}

	this->maximized = maximized;
}

void Window::setMinimized()
{
	if (this->baseWindow)
		SDL_MinimizeWindow(this->baseWindow);

	this->minimized = minimized;
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

	this->bordered = bordered;
}

void Window::setAlwaysOnTop(bool onTop)
{
	if (this->baseWindow)
		SDL_SetWindowAlwaysOnTop(this->baseWindow, onTop ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE);

	this->alwaysOnTop = onTop;
}

void Window::setResizable(bool resizable)
{
	if (this->baseWindow)
		SDL_SetWindowResizable(this->baseWindow, resizable ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE);

	this->resizable = resizable;
}

void Window::renderComponent(UIComponent *uicomponent)
{
	if (!uicomponent->isVisible())
		return;

	SDL_Texture *texture = SDL_CreateTexture(this->mainRenderer, SDL_PIXELFORMAT_RGBA32, SDL_RENDERER_TARGETTEXTURE, uicomponent->getSize().w, uicomponent->getSize().h);
	ENSURE_NOT(texture, NULL);

	ENSURE(SDL_UpdateTexture(texture, NULL, uicomponent->getSurface()->pixels, uicomponent->getSurface()->pitch), 0);

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

			int destx = childFrame->getPosition().x;
			int desty = childFrame->getPosition().y;
			int destw = srcw;
			int desth = srch;

			SDL_Rect rsrc = {srcx, srcy, srcw, srch};
			SDL_Rect rdest = {destx, desty, destw, desth};

			ENSURE(SDL_BlitSurface(childFrame->getSurface(), &rsrc, frame->getSurface(), &rdest), 0);
		}
		else
		{
			SDL_Rect rdest = {childComp->getPosition().x, childComp->getPosition().y, childComp->getSize().w, childComp->getSize().h};
			SDL_Rect rsrc = {0, 0, childComp->getSize().w, childComp->getSize().h};
			ENSURE(SDL_BlitSurface(childComp->getSurface(), &rsrc, frame->getSurface(), &rdest), 0);
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
	if (frame->hasFocus() && (RUIComponent *)frame != this->focusedComponent)
	{
		if (frame->focusTimeID > maxFocusTimeID)
			maxFocusTimeID = frame->focusTimeID;

		haveFocus.push_back(frame);
	}
}

void Window::handleFocusing()
{
	for (RUIComponent *comp : this->haveFocus)
	{
		if (comp->focusTimeID >= maxFocusTimeID)
		{
			if (this->focusedComponent)
			{
				this->focusedComponent->setFocus(false);
				this->focusChanged = true;
			}

			this->focusedComponent = comp;
		}
		else
		{
			comp->setFocus(false);
		}
	}

	this->haveFocus.clear();
}

void Window::addFrame(Frame *frame)
{
	this->framesQueue.push(frame);
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

void Window::onMousePressed(Event e)
{
}

void Window::onMouseReleased(Event e)
{
}

void Window::onKeyPressed(Event e)
{
}

void Window::onKeyReleased(Event e)
{
}
