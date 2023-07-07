#include "../headers/reluicomp.hpp"

RelativeUIComponent::RelativeUIComponent(UIComponent *parent, Uint16 width, Uint16 height) : UIComponent(width, height)
{
    this->parent = parent;
    this->onKeyPressedHandlerPtr = nullptr;
    this->onKeyReleasedHandlerPtr = nullptr;
    this->setFocus(false);
    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

Point RelativeUIComponent::getPosition()
{
    return {this->relX, this->relY};
}

UIComponent *RelativeUIComponent::getParent()
{
    return this->parent;
}

bool RelativeUIComponent::insideBounds(int x, int y)
{
    if (this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->getSize().w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->getSize().h)
    {
        if (this->getParent())
        {
            RelativeUIComponent *parent = dynamic_cast<RelativeUIComponent *>(this->getParent());
            if (parent)
                return (parent->visibleArea.x <= x && x <= parent->visibleArea.x + parent->visibleArea.w && parent->visibleArea.y <= y && y <= parent->visibleArea.y + parent->visibleArea.h);
        }
       return true;
    }
    return false;
}

bool RelativeUIComponent::hasFocus()
{
    return this->focus;
}

void RelativeUIComponent::setFocus(bool focus)
{
    this->focus = focus;
}

void RelativeUIComponent::updateVisibleArea(int x, int y, int width, int height)
{
    this->visibleArea.x = x;
    this->visibleArea.y = y;
    this->visibleArea.w = width;
    this->visibleArea.h = height;
}

void RelativeUIComponent::setSize(Uint16 width, Uint16 height)
{
    SDL_Surface *newBaseSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, baseSurface->format->BitsPerPixel, baseSurface->format->format);
    SDL_BlitSurface(baseSurface, NULL, newBaseSurface, NULL);
    SDL_FreeSurface(baseSurface);
    baseSurface = newBaseSurface;

    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

void RelativeUIComponent::setPosition(int x, int y)
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

bool RelativeUIComponent::invokeEvents(Event event)
{
    if (UIComponent::invokeEvents(event))
    {
        return true;
    }
    else if (event.type == EventType::KEY_PRESS)
    {
        this->onKeyPressed(event);
    }
    else if (event.type == EventType::KEY_RELEASE)
    {
        this->onKeyReleased(event);
    }
    else
    {
        return false;
    }

    return true;
}

RelativeUIComponent::~RelativeUIComponent()
{
}

// Event handlers

void RelativeUIComponent::setOnKeyPressedHandler(void (*handler)(Event e))
{
    this->onKeyPressedHandlerPtr = handler;
}

void RelativeUIComponent::setOnKeyReleasedHandler(void (*handler)(Event e))
{
    this->onKeyReleasedHandlerPtr = handler;
}

void RelativeUIComponent::onKeyPressed(Event e)
{
    if (this->onKeyPressedHandlerPtr)
        this->onKeyPressedHandlerPtr(e);
}

void RelativeUIComponent::onKeyReleased(Event e)
{
    if (this->onKeyReleasedHandlerPtr)
        this->onKeyReleasedHandlerPtr(e);
}
