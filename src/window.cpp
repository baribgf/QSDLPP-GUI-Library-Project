#include "../headers/window.hpp"

Window::Window(string title, int width, int height, bool fullscreen, bool centered)
{
    this->FPS = 90;
    this->frameDelay = 1000 / this->FPS;
    this->running = true;
    this->thereWasPendingEvent = false;
    this->focusedComponent = nullptr;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    int x = centered ? SDL_WINDOWPOS_CENTERED : randint(0, dm.w - width);
    int y = centered ? SDL_WINDOWPOS_CENTERED : randint(0, dm.h - height);

    this->baseWindow = SDL_CreateWindow(title.c_str(), x, y, width, height, (fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN) | SDL_WINDOW_RESIZABLE);
    this->mainRenderer = SDL_CreateRenderer(this->baseWindow, -1, 0);
    SDL_SetRenderDrawBlendMode(this->mainRenderer, SDL_BLENDMODE_BLEND);

    ////////////////////////////////////////////////////////////////////////////////////////
}

void Window::handleEvents()
{
    Event e = Event::toEvent(this->event);

    if (event.type == SDL_QUIT)
        this->running = false;

    else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        this->onWindowResized(e);

    else if (event.user.code == 2004)
        debug("Hello, World!");
}

void Window::mainloop()
{
    if (SDL_PollEvent(&this->event))
    {
        this->thereWasPendingEvent = true;
        this->handleEvents();
    }
    else
    {
        this->thereWasPendingEvent = false;
    }

    SDL_SetRenderDrawColor(this->mainRenderer, WHITE);
    SDL_RenderClear(this->mainRenderer);

    // do here ..

    for (Frame *frame : this->frames)
    {
        this->renderFrame(frame);
    }

    if (this->thereWasPendingEvent)
        this->handleFocusing();

    SDL_RenderPresent(this->mainRenderer);
}

void Window::exec()
{
    while (this->running)
    {
        int frameStart = SDL_GetTicks();

        this->mainloop();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);
    }

    for (Frame *frame : this->frames)
    {
        for (RUIComponent *comp : frame->getMembers())
        {
            SDL_FreeSurface(comp->getSDLSurface());
        }

        SDL_FreeSurface(frame->getSDLSurface());
    }

    this->frames.clear();
}

Point Window::getPosition()
{
    Point p;
    SDL_GetWindowPosition(this->baseWindow, &p.x, &p.y);
    return p;
}

Dimension Window::getSize()
{
    int w, h;
    SDL_GetWindowSize(this->baseWindow, &w, &h);
    return {(Uint16)w, (Uint16)h};
}

Uint16 Window::getFPS()
{
    return this->FPS;
}

void Window::setSize(Uint16 width, Uint16 height)
{
    SDL_SetWindowSize(this->baseWindow, width, height);
}

void Window::setPosition(int x, int y)
{
    SDL_SetWindowPosition(this->baseWindow, x, y);
}

void Window::setCentered()
{
    this->setPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window::setFPS(Uint16 fps)
{
    this->FPS = fps;
    this->frameDelay = 1000 / this->FPS;
}

void Window::renderComponent(UIComponent *uicomponent)
{
    if (!uicomponent->isVisible())
        return;

    int compx = uicomponent->getAbsPosition().x;
    int compy = uicomponent->getAbsPosition().y;
    int compw = uicomponent->getSize().w;
    int comph = uicomponent->getSize().h;

    SDL_Texture *texture = SDL_CreateTexture(
        this->mainRenderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_RENDERER_TARGETTEXTURE,
        compw,
        comph);

    SDL_UpdateTexture(texture, NULL, uicomponent->getSDLSurface()->pixels, uicomponent->getSDLSurface()->pitch);

    int destx = compx;
    int desty = compy;
    int destw = compw;
    int desth = comph;
    int srcx = 0;
    int srcy = 0;
    int srcw = compw;
    int srch = comph;

    RUIComponent *relUiComp = dynamic_cast<RUIComponent *>(uicomponent);
    if (relUiComp)
    {
        RUIComponent *relUiCompParent = dynamic_cast<RUIComponent *>(relUiComp->getParent());
        if (relUiCompParent)
        {
            int parx = relUiCompParent->visibleArea.x;
            int pary = relUiCompParent->visibleArea.y;
            int parw = relUiCompParent->visibleArea.w;
            int parh = relUiCompParent->visibleArea.h;

            if (compx + compw > parx + parw)
            {
                srcw = parw + parx - compx;
                destw = srcw;
            }
            if (compy + comph > pary + parh)
            {
                srch = parh + pary - compy;
                desth = srch;
            }
            if (compx < parx)
            {
                srcx = parx - compx;
                destx = parx;
                destw -= srcx;
            }
            if (compy < pary)
            {
                srcy = pary - compy;
                desty = pary;
                desth -= srcy;
            }
        }

        relUiComp->updateVisibleArea(destx, desty, destw, desth);
    }

    SDL_Rect rdest = {destx, desty, destw, desth};
    SDL_Rect rsrc = {srcx, srcy, srcw, srch};

    SDL_RenderCopy(this->mainRenderer, texture, &rsrc, &rdest);
    SDL_DestroyTexture(texture);

    if (this->thereWasPendingEvent)
    {
        uicomponent->invokeEvents(Event::toEvent(this->event));
    }
}

void Window::renderFrame(Frame *frame)
{
    this->renderComponent(frame);

    for (RUIComponent *childComp : frame->getMembers())
    {
        this->renderComponent(childComp);
        if (childComp->hasFocus() && childComp != this->focusedComponent)
        {
            if (childComp->focusTimeID > maxFocusTimeID)
                maxFocusTimeID = childComp->focusTimeID;

            haveFocus.push_back(childComp);
        }

        Frame *childFrame = dynamic_cast<Frame *>(childComp);
        if (childFrame)
            this->renderFrame(childFrame);
    }

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

void Window::delFrame(Frame *frame)
{
    for (int i = 0; i < this->frames.size(); i++)
    {
        if (this->frames.at(i) == frame)
            this->frames.erase(this->frames.begin() + i);
    }
}

Window::~Window()
{
    SDL_DestroyRenderer(this->mainRenderer);
    SDL_DestroyWindow(this->baseWindow);
    TTF_Quit();
    SDL_Quit();
}

// Event handlers

void Window::onWindowResized(Event e)
{
}
