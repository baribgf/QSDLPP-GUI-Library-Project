#include "../../headers/UIComponents/button.hpp"

Button::Button(RUIComponent *parent, string text, int width, int height) : Label(parent, text, width, height)
{
    this->DEFAULT_HOVER_BG = {BLACK};
    this->DEFAULT_HOVER_FG = {WHITE};
    this->bordersColor = this->DEFAULT_BORDERS_COLOR;
    this->setBg(this->DEFAULT_BG);
    this->setFg(this->DEFAULT_FG);
    this->setBordersVisible(true);
    this->setTextAlign(Align::CENTER);
}

Color Button::getBorderColor()
{
    return this->bordersColor;
}

void Button::setBorderColor(Color color)
{
    this->bordersColor = color;
    this->drawBorders();
}

void Button::onClick(Event e)
{
    Label::onClick(e);
}

void Button::onMouseEntered(Event e)
{
    this->setBg(this->DEFAULT_HOVER_BG);
    this->setFg(this->DEFAULT_HOVER_FG);
    SDL_SetCursor(HAND_CURSOR);
    SDL_ShowCursor(SDL_ENABLE);

    if (this->onMouseEnteredHandlerPtr)
        this->onMouseEnteredHandlerPtr(e);
}

void Button::onMouseLeaved(Event e)
{
    this->setBg(this->DEFAULT_BG);
    this->setFg(this->DEFAULT_FG);
    SDL_SetCursor(DEFAULT_CURSOR);
    SDL_ShowCursor(SDL_ENABLE);
    
    if (this->onMouseLeavedHandlerPtr != nullptr)
        this->onMouseLeavedHandlerPtr(e);
}

void Button::onKeyReleased(Event e)
{
    Label::onKeyReleased(e);

    if (e.key.type == KeyCode::KEY_RETURN)
        this->onClick(e);
}

void Button::setFocus(bool focus)
{
    if (focus)
    {
        this->focusTimeID = SDL_GetTicks64();
        this->setBorderColor({WHITE});
    }
    else
    {
        this->setBorderColor(this->DEFAULT_BORDERS_COLOR);
    }

    this->focus = focus;
}
