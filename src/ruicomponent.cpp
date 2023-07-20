#include "../QSDL++/RUIComponent"

RUIComponent::RUIComponent(UIComponent *parent, Uint16 width, Uint16 height) : UIComponent(width, height)
{
    this->parent = parent;
    this->focusTimeID = 0;
    this->setFocus(false);
    this->onKeyPressedHandlerPtr = nullptr;
    this->onKeyReleasedHandlerPtr = nullptr;
    this->setPosition(0, 0);
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
    RUIComponent *parent = dynamic_cast<RUIComponent *>(this->getParent());
    if (parent)
    {
        if (!(
            this->getAbsPosition().x - parviewport.x <= x && x <= this->getAbsPosition().x + this->getSize().w - parviewport.x
            && this->getAbsPosition().y - parviewport.y <= y && y <= this->getAbsPosition().y + this->getSize().h - parviewport.y))
            return false;

        if (!(
            parent->getAbsPosition().x <= x && x <= parent->getAbsPosition().x + parviewport.w
            && parent->getAbsPosition().y <= y && y <= parent->getAbsPosition().y + parviewport.h))
            return false;
    }
    else
    {
        if (!(
            this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->getSize().w
            && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->getSize().h))
            return false;
    }

    return true;
}

bool RUIComponent::hasFocus()
{
    return this->focus;
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

void RUIComponent::setOnKeyPressedHandler(void (*handler)(Event e, void*), void *data)
{
    this->onKeyPressedHandlerPtr = handler;
    this->keyPressHandlerData = data;
}

void RUIComponent::setOnKeyReleasedHandler(void (*handler)(Event e, void*), void *data)
{
    this->onKeyReleasedHandlerPtr = handler;
    this->keyReleaseHandlerData = data;
}

void RUIComponent::onKeyPressed(Event e)
{
    if (this->onKeyPressedHandlerPtr)
        this->onKeyPressedHandlerPtr(e, keyPressHandlerData);
}

void RUIComponent::onKeyReleased(Event e)
{
    if (this->onKeyReleasedHandlerPtr)
        this->onKeyReleasedHandlerPtr(e, keyReleaseHandlerData);
}

void RUIComponent::onClick(Event e)
{
    UIComponent::onClick(e);

    this->setFocus(true);

    Event event;
    event.type = EventType::USER_EVENT;
    event.push();
}
