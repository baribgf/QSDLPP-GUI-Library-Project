#include "../../headers/UIComponents/label.hpp"

Label::Label(RelativeUIComponent *parent, string text, int width, int height) : RelativeUIComponent(parent, width, height), text(text)
{
    this->textX = 0;
    this->textY = 0;
    this->alignType = Align::NORTH_WEST;
    this->bordersVisible = false;
    this->FONT_PATH = "/usr/share/fonts/JetBrains/ttf/JetBrainsMono-Regular.ttf";
    this->DEFAULT_BG = {WHITE};
    this->DEFAULT_FG = {BLACK};
    this->DEFAULT_BORDER_COLOR = {BLACK};
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

Color Label::getFg()
{
    return this->fg;
}

Color Label::getBg()
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
    this->update(text, this->ptSize, this->fg);
}

void Label::update(string text, Uint8 ptSize, Color fg)
{
    this->fill(this->fillColor);

    SDL_Rect r = {this->textX, this->textY, this->getSize().w, this->getSize().h};
    TTF_Font *font = TTF_OpenFont(this->FONT_PATH.c_str(), ptSize);
    SDL_Surface *textSurface = TTF_RenderText_Blended(
        font,
        text.c_str(),
        {this->fg.r, this->fg.g, this->fg.b, this->fg.a});

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

        drawLineToSurface( // top line
            this->baseSurface,
            0, 0,
            this->getSize().w, 0,
            color);

        drawLineToSurface( // bottom line
            this->baseSurface,
            0, this->getSize().h - 1,
            this->getSize().w, this->getSize().h - 1,
            color);

        drawLineToSurface( // left line
            this->baseSurface,
            0, 0,
            0, this->getSize().h,
            color);

        drawLineToSurface( // right line
            this->baseSurface,
            this->getSize().w - 1, 0,
            this->getSize().w - 1, this->getSize().h - 1,
            color);
    }
}

void Label::setFg(Color fg)
{
    this->fg = fg;
    this->update(this->text, this->ptSize, fg);
}

void Label::setBg(Color bg)
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
