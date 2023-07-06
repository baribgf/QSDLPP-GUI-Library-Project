#include "../../headers/UIComponents/button.hpp"

Button::Button(RelativeUIComponent *parent, string text, int width, int height) : Label(parent, text, width, height)
{
    this->DEFAULT_HOVER_BG = toSDLColor(BLACK);
    this->DEFAULT_HOVER_FG = toSDLColor(WHITE);
    this->borderColor = this->DEFAULT_BORDER_COLOR;
    this->setBg(this->DEFAULT_BG);
    this->setFg(this->DEFAULT_FG);
    this->setBordersVisible(true);
}

SDL_Color Button::getBorderColor()
{
    return this->borderColor;
}

void Button::setBorderColor(SDL_Color color)
{
    this->borderColor = color;
    this->updateBorders();
}

void Button::onClick(Event e)
{
    if (this->onClickHandlerPtr)
        this->onClickHandlerPtr(e);
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
