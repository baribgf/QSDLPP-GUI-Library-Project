#include "../../QSDL++/UIComponents/CheckBox"

CheckBox::CheckBox(RUIComponent *parent, string text, int width, int height) : Label(parent, text, width, height)
{
    this->boxColor = {55, 55, 55, 255};
    this->checked = false;
    this->cbW = 18;
    this->cbH = 18;
    this->cbX = 0.04 * width;
    this->cbY = (height - this->cbH) / 2;
    this->setTextAlign(Align::CENTER);
}

void CheckBox::update()
{
    Label::update();

    this->drawCheckBox();
}

bool CheckBox::isChecked()
{
    return this->checked;
}

void CheckBox::drawCheckBox()
{
    Uint32 color = SDL_MapRGBA(this->baseSurface->format, this->boxColor.r, this->boxColor.g, this->boxColor.b, this->boxColor.a);

    drawLineToSurface( // top line
        this->baseSurface,
        this->cbX, this->cbY,
        this->cbX + this->cbW, this->cbY,
        color);

    drawLineToSurface( // bottom line
        this->baseSurface,
        this->cbX, this->cbY + this->cbH - 1,
        this->cbX + this->cbW, this->cbY + this->cbH - 1,
        color);

    drawLineToSurface( // left line
        this->baseSurface,
        this->cbX, this->cbY,
        this->cbX, this->cbY + this->cbH,
        color);

    drawLineToSurface( // right line
        this->baseSurface,
        this->cbX + this->cbW - 1, this->cbY,
        this->cbX + this->cbW - 1, this->cbY + this->cbH,
        color);

    if (this->checked)
    {
        int hpad = 3;
        int vpad = 3;
        SDL_Surface *s = SDL_CreateRGBSurfaceWithFormat(0, this->cbW - 2 * hpad, this->cbH - 2 * vpad, this->baseSurface->format->BitsPerPixel, this->baseSurface->format->format);
        ENSURE_NOT(s, NULL);
        ENSURE(SDL_FillRect(s, NULL, color), 0);
        SDL_Rect r = {this->cbX + hpad, this->cbY + vpad, s->w, s->h};
        ENSURE(SDL_BlitSurface(s, NULL, this->baseSurface, &r), 0);
    }
}

void CheckBox::setTextAlign(Align a)
{
    switch (a)
    {
    case Align::CENTER:
        this->textAlign = Align::CENTER;
        this->textX = (this->getSize().w - this->textW + this->cbX + this->cbW) / 2;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::EAST:
        this->textAlign = Align::EAST;
        this->textX = this->getSize().w - this->textW + this->cbX + this->cbW;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::WEST:
        this->textAlign = Align::WEST;
        this->textX = 0;
        this->textY = (this->getSize().h - this->textH) / 2;
        break;

    case Align::NORTH:
        this->textAlign = Align::NORTH;
        this->textX = (this->getSize().w - this->textW + this->cbX + this->cbW) / 2;
        this->textY = 0;
        break;

    case Align::SOUTH:
        this->textAlign = Align::SOUTH;
        this->textX = (this->getSize().w - this->textW + this->cbX + this->cbW) / 2;
        this->textY = this->getSize().h - this->textH;
        break;

    case Align::NORTH_EAST:
        this->textAlign = Align::NORTH_EAST;
        this->textX = this->getSize().w - this->textW + this->cbX + this->cbW;
        this->textY = 0;
        break;

    case Align::NORTH_WEST:
        this->textAlign = Align::NORTH_WEST;
        this->textX = 0;
        this->textY = 0;
        break;

    case Align::SOUTH_EAST:
        this->textAlign = Align::SOUTH_EAST;
        this->textX = this->getSize().w - this->textW + this->cbX + this->cbW;
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

void CheckBox::setChecked(bool checked)
{
    this->checked = checked;
    this->update();
}

void CheckBox::setBoxColor(Color c)
{
    this->boxColor = c;
    this->update();
}

CheckBox::~CheckBox()
{
}

void CheckBox::onClick(Event e)
{
    Label::onClick(e);

    int posX = this->getAbsPosition().x + this->cbX;
    int posY = this->getAbsPosition().y + this->cbY;
    int sizeW = this->cbW;
    int sizeH = this->cbH;

    if (posX <= e.button.x && e.button.x <= posX + sizeW
        && posY <= e.button.y && e.button.y <= posY + sizeH)
    {
        if (this->isChecked())
            this->setChecked(false);
        else
            this->setChecked(true);
    }
}
