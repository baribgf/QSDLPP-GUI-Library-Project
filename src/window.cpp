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
    {
        this->running = false;
    }

    else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        this->onWindowResized(e);
    }

    else if (event.user.code == 2004)
    {
        debug("Hello, World!");
    }
    
    int step = 5;

    if (event.key.keysym.sym == SDLK_RIGHT && event.type == SDL_KEYDOWN)
    {
        Frame *f = this->frames.at(0);
        f->setViewport(f->viewport.x + step, f->viewport.y, f->viewport.w, f->viewport.h);
    }
    else if (event.key.keysym.sym == SDLK_LEFT && event.type == SDL_KEYDOWN)
    {
        Frame *f = this->frames.at(0);
        f->setViewport(f->viewport.x - step, f->viewport.y, f->viewport.w, f->viewport.h);
    }
    else if (event.key.keysym.sym == SDLK_DOWN && event.type == SDL_KEYDOWN)
    {
        Frame *f = this->frames.at(0);
        if (f->viewport.y + step <= f->getSize().h - f->viewport.h)
            f->setViewport(f->viewport.x, f->viewport.y + step, f->viewport.w, f->viewport.h);
    }
    else if (event.key.keysym.sym == SDLK_UP && event.type == SDL_KEYDOWN)
    {
        Frame *f = this->frames.at(0);
        f->setViewport(f->viewport.x, f->viewport.y - step, f->viewport.w, f->viewport.h);
    }
    
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
            SDL_FreeSurface(comp->getSDLSurface());

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

    int srcx = 0;
    int srcy = 0;
    int srcw = compw;
    int srch = comph;

    int destx = compx;
    int desty = compy;
    int destw = srcw;
    int desth = srch;

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

        // relUiComp->updateVisibleArea(destx, desty, destw, desth);
    }

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

    SDL_RenderCopy(this->mainRenderer, texture, &rsrc, &rdest);
    SDL_DestroyTexture(texture);

    if (this->thereWasPendingEvent)
    {
        uicomponent->invokeEvents(Event::toEvent(this->event));
    }
}

void Window::renderFrame(Frame *frame)
{
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
        }
        else
        {
            SDL_Rect rdest = {
                childComp->getPosition().x,
                childComp->getPosition().y,
                childComp->getSize().w,
                childComp->getSize().h};
            
            SDL_Rect rsrc = {0, 0, childComp->getSize().w, childComp->getSize().h};
            SDL_BlitSurface(childComp->getSDLSurface(), &rsrc, frame->getSDLSurface(), &rdest);
        }

        if (this->thereWasPendingEvent)
        {
            childComp->invokeEvents(Event::toEvent(this->event));
        }

        if (childComp->hasFocus() && childComp != this->focusedComponent)
        {
            if (childComp->focusTimeID > maxFocusTimeID)
                maxFocusTimeID = childComp->focusTimeID;

            haveFocus.push_back(childComp);
        }
    }

    frame->update(); // update the frame before blitting into it.

    this->renderComponent(frame);

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
