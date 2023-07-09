#include "../headers/reluicomp.hpp"

RUIComponent::RUIComponent(UIComponent *parent, Uint16 width, Uint16 height) : UIComponent(width, height)
{
    this->parent = parent;
    this->focusTimeID = 0;
    this->setFocus(false);
    this->onKeyPressedHandlerPtr = nullptr;
    this->onKeyReleasedHandlerPtr = nullptr;
    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

Point RUIComponent::getPosition()
{
    return {this->relX, this->relY};
}

UIComponent *RUIComponent::getParent()
{
    return this->parent;
}

bool RUIComponent::insideBounds(int x, int y)
{
    if (this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->getSize().w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->getSize().h)
    {
        if (this->getParent())
        {
            RUIComponent *parent = dynamic_cast<RUIComponent *>(this->getParent());
            if (parent)
                return (parent->visibleArea.x <= x && x <= parent->visibleArea.x + parent->visibleArea.w && parent->visibleArea.y <= y && y <= parent->visibleArea.y + parent->visibleArea.h);
        }
        return true;
    }
    return false;
}

bool RUIComponent::hasFocus()
{
    return this->focus;
}

void RUIComponent::updateVisibleArea(int x, int y, int width, int height)
{
    this->visibleArea.x = x;
    this->visibleArea.y = y;
    this->visibleArea.w = width;
    this->visibleArea.h = height;
}

void RUIComponent::setSize(Uint16 width, Uint16 height)
{
    SDL_Surface *newBaseSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, baseSurface->format->BitsPerPixel, baseSurface->format->format);
    SDL_BlitSurface(baseSurface, NULL, newBaseSurface, NULL);
    SDL_FreeSurface(baseSurface);
    baseSurface = newBaseSurface;

    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

void RUIComponent::setPosition(int x, int y)
{
    this->relX = x;
    this->relY = y;

    if (this->parent)
    {
        Point parPos = this->parent->getAbsPosition();
        x += parPos.x;
        y += parPos.y;
    }

    this->setAbsPosition(x, y);

    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

void RUIComponent::setFocus(bool focus)
{
    if (focus)
        this->focusTimeID = SDL_GetTicks64();

    this->focus = focus;
}

void RUIComponent::invokeEvents(Event event)
{
    UIComponent::invokeEvents(event);
    
    if (this->hasFocus())
    {
        if (event.type == EventType::KEY_PRESS)
        {
            this->onKeyPressed(event);
        }
        else if (event.type == EventType::KEY_RELEASE)
        {
            this->onKeyReleased(event);
        }
    }
}

RUIComponent::~RUIComponent()
{
}

// Event handlers

void RUIComponent::setOnKeyPressedHandler(void (*handler)(Event e))
{
    this->onKeyPressedHandlerPtr = handler;
}

void RUIComponent::setOnKeyReleasedHandler(void (*handler)(Event e))
{
    this->onKeyReleasedHandlerPtr = handler;
}

void RUIComponent::onKeyPressed(Event e)
{
    if (this->onKeyPressedHandlerPtr)
        this->onKeyPressedHandlerPtr(e);
}

void RUIComponent::onKeyReleased(Event e)
{
    if (this->onKeyReleasedHandlerPtr)
        this->onKeyReleasedHandlerPtr(e);
}

void RUIComponent::onClick(Event e)
{
    UIComponent::onClick(e);

    this->setFocus(true);
}
