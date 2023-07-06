#include "../../headers/UIComponents/label.hpp"

Label::Label(RelativeUIComponent *parent, string text, int width, int height, SDL_Color fg, SDL_Color bg) : RelativeUIComponent(parent, width, height, bg), text(text), fg(fg)
{
    this->textX = 0;
    this->textY = 0;
    this->alignType = Align::NORTH_WEST;
    this->bordersVisible = false;
    this->FONT_PATH = "../resources/JetBrainsMono-Regular.ttf";
    this->DEFAULT_BG = toSDLColor(WHITE);
    this->DEFAULT_FG = toSDLColor(BLACK);
    this->DEFAULT_BORDER_COLOR = toSDLColor(BLACK);
    this->ptSize = pixelsToPoints(height);

    this->setPosition(0, 0);

    SDL_Rect r = {this->textX, this->textY, width, height};
    this->update(text, ptSize, fg);
    this->setTextAlign(Align::CENTER);
}

string Label::getText()
{
    return this->text;
}

SDL_Color Label::getFg()
{
    return this->fg;
}

SDL_Color Label::getBg()
{
    return this->getFillColor();
}

Uint8 Label::getPtSize()
{
    return this->ptSize;
}

Align Label::getAlign()
{
    return this->alignType;
}

void Label::setTextAlign(Align a)
{
    switch (a)
    {
    case Align::CENTER:
        this->alignType = Align::CENTER;
        this->textX = (this->getSize().w - this->textW) / 2;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::EAST:
        this->alignType = Align::EAST;
        this->textX = this->getSize().w - this->textW;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::WEST:
        this->alignType = Align::WEST;
        this->textX = 0;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::NORTH:
        this->alignType = Align::NORTH;
        this->textX = (this->getSize().w - this->textW) / 2;
        this->textY = 0;
        break;

    case Align::SOUTH:
        this->alignType = Align::SOUTH;
        this->textX = (this->getSize().w - this->textW) / 2;
        this->textY = this->getSize().h - this->textH;
        break;

    case Align::NORTH_EAST:
        this->alignType = Align::NORTH_EAST;
        this->textX = this->getSize().w - this->textW;
        this->textY = 0;
        break;

    case Align::NORTH_WEST:
        this->alignType = Align::NORTH_WEST;
        this->textX = 0;
        this->textY = 0;
        break;

    case Align::SOUTH_EAST:
        this->alignType = Align::SOUTH_EAST;
        this->textX = this->getSize().w - this->textW;
        this->textY = this->getSize().h - this->textH;
        break;

    case Align::SOUTH_WEST:
        this->alignType = Align::SOUTH_WEST;
        this->textX = 0;
        this->textY = this->getSize().h - this->textH;
        break;
    }

    this->update(this->text, this->ptSize, this->fg);
}

void Label::setText(string text)
{
    this->text = text;
}

void Label::update(string text, Uint8 ptSize, SDL_Color fg)
{
    this->fill(this->fillColor);

    SDL_Rect r = {this->textX, this->textY, this->getSize().w, this->getSize().h};
    TTF_Font *font = TTF_OpenFont(this->FONT_PATH.c_str(), ptSize);
    SDL_Surface *textSurface = TTF_RenderText_Blended(
        font,
        text.c_str(),
        fg);

    SDL_BlitSurface(
        textSurface,
        NULL,
        this->baseSurface,
        &r);

    this->textW = textSurface->w;
    this->textH = textSurface->h;

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);

    this->updateBorders();
}

void Label::updateBorders()
{
    if (this->bordersVisible)
    {
        // drawing borders
        Uint32 color = SDL_MapRGBA(this->baseSurface->format, this->borderColor.r, this->borderColor.g, this->borderColor.b, this->borderColor.a);
        drawLineToSurface(this->baseSurface, 0, 0, this->getSize().w, 0, color);
        drawLineToSurface(this->baseSurface, 0, this->getSize().h - 1, this->getSize().w - 1, this->getSize().h - 1, color);
        drawLineToSurface(this->baseSurface, 0, 0, 0, this->getSize().h - 1, color);
        drawLineToSurface(this->baseSurface, this->getSize().w - 1, 0, this->getSize().w - 1, this->getSize().h - 1, color);
    }
}

void Label::setFg(SDL_Color fg)
{
    this->fg = fg;
    this->update(this->text, this->ptSize, fg);
}

void Label::setBg(SDL_Color bg)
{
    this->fillColor = bg;

    this->setFg(this->fg);
}

void Label::setPtSize(Uint8 p)
{
    this->ptSize = p;
    this->update(this->text, p, this->fg);
    this->setTextAlign(this->alignType);
}

void Label::setBordersVisible(bool visible)
{
    this->bordersVisible = visible;
    this->update(this->text, this->ptSize, this->fg);
}
