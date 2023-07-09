#include "../../headers/UIComponents/label.hpp"

Label::Label(RUIComponent *parent, string text, int width, int height) : RUIComponent(parent, width, height), text(text)
{
    this->textX = 0;
    this->textY = 0;
    this->textW = 0;
    this->textH = height;
    this->imgX = 0;
    this->imgY = 0;
    this->autoImgSize = true;
    this->imgPath = "";
    this->textAlign = Align::NORTH_WEST;
    this->bordersVisible = false;
    this->FONT_PATH = "/usr/share/fonts/JetBrains/ttf/JetBrainsMono-Regular.ttf";
    this->DEFAULT_BG = {WHITE};
    this->DEFAULT_FG = {BLACK};
    this->DEFAULT_BORDERS_COLOR = {BLACK};
    
    this->setTextSize(pixelsToPoints(height));
    this->setPosition(0, 0);
    this->setFg(this->DEFAULT_FG);
    this->setBordersColor(this->DEFAULT_BORDERS_COLOR);

    SDL_Rect r = {this->textX, this->textY, width, height};
    this->update();
    this->setTextAlign(this->textAlign);
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

Uint8 Label::getTextSize()
{
    return this->textSize;
}

Align Label::getAlign()
{
    return this->textAlign;
}

Color Label::getBordersColor()
{
    return this->bordersColor;
}

Dimension Label::getImageSize()
{
    return {this->imgW, this->imgH};
}

void Label::setTextAlign(Align a)
{
    switch (a)
    {
    case Align::CENTER:
        this->textAlign = Align::CENTER;
        this->textX = (this->getSize().w - this->textW) / 2;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::EAST:
        this->textAlign = Align::EAST;
        this->textX = this->getSize().w - this->textW;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::WEST:
        this->textAlign = Align::WEST;
        this->textX = 0;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::NORTH:
        this->textAlign = Align::NORTH;
        this->textX = (this->getSize().w - this->textW) / 2;
        this->textY = 0;
        break;

    case Align::SOUTH:
        this->textAlign = Align::SOUTH;
        this->textX = (this->getSize().w - this->textW) / 2;
        this->textY = this->getSize().h - this->textH;
        break;

    case Align::NORTH_EAST:
        this->textAlign = Align::NORTH_EAST;
        this->textX = this->getSize().w - this->textW;
        this->textY = 0;
        break;

    case Align::NORTH_WEST:
        this->textAlign = Align::NORTH_WEST;
        this->textX = 0;
        this->textY = 0;
        break;

    case Align::SOUTH_EAST:
        this->textAlign = Align::SOUTH_EAST;
        this->textX = this->getSize().w - this->textW;
        this->textY = this->getSize().h - this->textH;
        break;

    case Align::SOUTH_WEST:
        this->textAlign = Align::SOUTH_WEST;
        this->textX = 0;
        this->textY = this->getSize().h - this->textH;
        break;
    }

    this->update();
}

void Label::setText(string text)
{
    this->text = text;
    this->update();
    this->setTextAlign(this->getAlign());
}

void Label::update()
{
    this->fill(this->fillColor);

    this->drawBorders();

    if (this->imgPath.length() > 0)
        this->drawImage();

    this->drawText();
}

void Label::drawText()
{
    if (text.length() > 0)
    {
        SDL_Rect r = {this->textX, this->textY, this->getSize().w, this->getSize().h};
        TTF_Font *font = TTF_OpenFont(this->FONT_PATH.c_str(), this->textSize);

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

        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
    }
    else
    {
        this->textW = 0;
    }
}

void Label::drawBorders()
{
    if (this->bordersVisible)
    {
        // drawing borders
        Uint32 color = SDL_MapRGBA(this->baseSurface->format, this->bordersColor.r, this->bordersColor.g, this->bordersColor.b, this->bordersColor.a);

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
    this->update();
}

void Label::setBg(Color bg)
{
    this->fillColor = bg;

    this->setFg(this->fg);
}

void Label::setTextSize(Uint8 p)
{
    this->textSize = p;
    this->update(); // mandatory for updaing text dimensions.
    this->setTextAlign(this->textAlign);
}

void Label::setBordersVisible(bool visible)
{
    this->bordersVisible = visible;
    this->update();
}

void Label::setBordersColor(Color color)
{
    this->bordersColor = color;
    this->drawBorders();
}

void Label::drawImage()
{
    SDL_Surface *imgSurface = IMG_Load(this->imgPath.c_str());
    Point pos = this->getPosition();
    Dimension size = this->getSize();
    SDL_Rect destRect;
    destRect.x = imgX;
    destRect.y = imgY;

    if (this->autoImgSize)
    {
        if (size.w > size.h)
        {
            destRect.h = size.h;
            destRect.w = destRect.h * imgSurface->w / imgSurface->h;
        }
        else
        {
            destRect.w = size.w;
            destRect.h = destRect.w * imgSurface->h / imgSurface->w;
        }

        this->imgW = destRect.w;
        this->imgH = destRect.h;
    }
    else
    {
        destRect.w = this->imgW;
        destRect.h = this->imgH;
    }

    SDL_BlitScaled(imgSurface, NULL, this->baseSurface, &destRect);
    SDL_FreeSurface(imgSurface);
}

void Label::setImage(string imgPath, Align align)
{
    this->imgPath = imgPath;
    this->imgAlign = align;
    this->update();
    this->alignImage();
}

void Label::alignImage()
{
    switch (this->imgAlign)
    {
    case Align::CENTER:
        this->imgAlign = Align::CENTER;
        this->imgX = (this->getSize().w - this->imgW) / 2;
        this->imgY = (this->getSize().h - this->imgH) / 2;
        break;

    case Align::EAST:
        this->imgAlign = Align::EAST;
        this->imgX = this->getSize().w - this->imgW;
        this->imgY = (this->getSize().h - this->imgH) / 2;
        break;

    case Align::WEST:
        this->imgAlign = Align::WEST;
        this->imgX = 0;
        this->imgY = (this->getSize().h - this->imgH) / 2;
        break;

    case Align::NORTH:
        this->imgAlign = Align::NORTH;
        this->imgX = (this->getSize().w - this->imgW) / 2;
        this->imgY = 0;
        break;

    case Align::SOUTH:
        this->imgAlign = Align::SOUTH;
        this->imgX = (this->getSize().w - this->imgW) / 2;
        this->imgY = this->getSize().h - this->imgH;
        break;

    case Align::NORTH_EAST:
        this->imgAlign = Align::NORTH_EAST;
        this->imgX = this->getSize().w - this->imgW;
        this->imgY = 0;
        break;

    case Align::NORTH_WEST:
        this->imgAlign = Align::NORTH_WEST;
        this->imgX = 0;
        this->imgY = 0;
        break;

    case Align::SOUTH_EAST:
        this->imgAlign = Align::SOUTH_EAST;
        this->imgX = this->getSize().w - this->imgW;
        this->imgY = this->getSize().h - this->imgH;
        break;

    case Align::SOUTH_WEST:
        this->imgAlign = Align::SOUTH_WEST;
        this->imgX = 0;
        this->imgY = this->getSize().h - this->imgH;
        break;
    }

    this->update();
}

void Label::setImageSize(Uint32 width, Uint32 height)
{
    this->autoImgSize = false;
    this->imgW = width;
    this->imgH = height;
    this->update();
}

void Label::setImageSizeAuto(bool _auto)
{
    this->autoImgSize = _auto;
    this->update();
}
