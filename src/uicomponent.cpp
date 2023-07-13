#include "../headers/uicomponent.hpp"

UIComponent::UIComponent(Uint16 width, Uint16 height)
{
    this->fillColor = {WHITE};
    this->mouseEntered = false;
    this->mousePressed = false;
    this->mouseReleased = true;
    this->visible = true;

    this->onClickHandlerPtr = nullptr;
    this->onMousePressedHandlerPtr = nullptr;
    this->onMouseReleasedHandlerPtr = nullptr;
    this->onMouseEnteredHandlerPtr = nullptr;
    this->onMouseLeavedHandlerPtr = nullptr;
    this->onMouseMotionHandlerPtr = nullptr;

    this->DEFAULT_CURSOR = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    ENSURE_NOT(this->DEFAULT_CURSOR, NULL);
    this->HAND_CURSOR = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    ENSURE_NOT(this->HAND_CURSOR, NULL);
    this->baseSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    ENSURE_NOT(this->baseSurface, NULL);

    this->setAbsPosition(0, 0);
    this->fill(fillColor);
}

bool UIComponent::isVisible()
{
    return this->visible;
}

Color UIComponent::getFillColor()
{
    return this->fillColor;
}

SDL_Surface *UIComponent::getSDLSurface()
{
    return this->baseSurface;
}

Dimension UIComponent::getSize()
{
    return {(Uint16)this->baseSurface->w, (Uint16)this->baseSurface->h};
}

Point UIComponent::getAbsPosition()
{
    return {this->x, this->y};
}

void UIComponent::setSize(Uint16 width, Uint16 height)
{
    SDL_Surface *newBaseSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, baseSurface->format->BitsPerPixel, baseSurface->format->format);
    ENSURE_NOT(newBaseSurface, NULL);
    ENSURE(SDL_BlitSurface(baseSurface, NULL, newBaseSurface, NULL), 0);
    SDL_FreeSurface(baseSurface);
    baseSurface = newBaseSurface;
}

void UIComponent::setVisible(bool visible)
{
    this->visible = visible;
}

void UIComponent::setAbsPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void UIComponent::fill(Color color)
{
    ENSURE(SDL_FillRect(this->baseSurface, NULL, SDL_MapRGBA(this->baseSurface->format, color.r, color.g, color.b, color.a)), 0);
    this->fillColor = color;
}

bool UIComponent::insideBounds(int x, int y)
{
    return (this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->getSize().w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->getSize().h);
}

void UIComponent::invokeEvents(Event event)
{
    if (this->insideBounds(event.button.x, event.button.y))
    {
        if (event.type == SDL_MOUSEMOTION)
        {
            this->onMouseMotion(event);

            if (!this->mouseEntered)
            {
                this->onMouseEntered(event);
                this->mouseEntered = true;
            }
        }
        else if (event.button.type == SDL_MOUSEBUTTONDOWN)
        {
            this->mousePressed = true;
            this->mouseReleased = false;
            this->onMousePressed(event);
        }
        else if (event.button.type == SDL_MOUSEBUTTONUP)
        {
            this->mouseReleased = true;
            this->onMouseReleased(event);

            if (this->mousePressed)
            {
                this->mousePressed = false;
                this->onClick(event);
            }
        }
    }
    else if (this->mouseEntered)
    {
        this->mouseEntered = false;
        this->onMouseLeaved(event);
    }
}

UIComponent::~UIComponent()
{
}

// Event handlers

void UIComponent::setOnClickHandler(void (*handler)(Event e))
{
    this->onClickHandlerPtr = handler;
}

void UIComponent::setOnMousePressedHandler(void (*handler)(Event e))
{
    this->onMousePressedHandlerPtr = handler;
}

void UIComponent::setOnMouseReleasedHandler(void (*handler)(Event e))
{
    this->onMouseReleasedHandlerPtr = handler;
}

void UIComponent::setOnMouseEnteredHandler(void (*handler)(Event e))
{
    this->onMouseEnteredHandlerPtr = handler;
}

void UIComponent::setOnMouseLeavedHandler(void (*handler)(Event e))
{
    this->onMouseLeavedHandlerPtr = handler;
}

void UIComponent::setOnMouseMotionHandler(void (*handler)(Event e))
{
    this->onMouseMotionHandlerPtr = handler;
}

void UIComponent::onClick(Event e)
{
    if (this->onClickHandlerPtr)
        this->onClickHandlerPtr(e);
}

void UIComponent::onMousePressed(Event e)
{
    if (this->onMousePressedHandlerPtr)
        this->onMousePressedHandlerPtr(e);
}

void UIComponent::onMouseReleased(Event e)
{
    if (this->onMouseReleasedHandlerPtr)
        this->onMouseReleasedHandlerPtr(e);
}

void UIComponent::onMouseEntered(Event e)
{
    if (this->onMouseEnteredHandlerPtr)
        this->onMouseEnteredHandlerPtr(e);
}

void UIComponent::onMouseLeaved(Event e)
{
    if (this->onMouseLeavedHandlerPtr)
        this->onMouseLeavedHandlerPtr(e);
}

void UIComponent::onMouseMotion(Event e)
{
    if (this->onMouseMotionHandlerPtr)
        this->onMouseMotionHandlerPtr(e);
}
