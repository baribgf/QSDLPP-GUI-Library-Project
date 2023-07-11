#include "../../headers/UIComponents/frame.hpp"

Frame::Frame(RUIComponent *parent, int width, int height) : RUIComponent(parent, width, height)
{
    this->overflowX = true;
    this->overflowY = true;
    this->movingScrollBarV = false;
    this->movingScrollBarH = false;
    this->scrollBarMovingDistY = 0;
    this->scrollBarMovingStartY = 0;
    this->scrollBarMovingDistX = 0;
	this->scrollBarMovingStartX = 0;
    this->viewport = {0, 0, width, height};
    this->DEFAULT_SCROLL_BAR_WIDTH = 15;
}

Color Frame::getBg()
{
    return this->bg;
}

bool Frame::isOverflowX()
{
    return this->overflowX;
}

bool Frame::isOverflowY()
{
    return this->overflowY;
}

bool Frame::insideBounds(int x, int y)
{
    for (RUIComponent *comp : this->members)
    {
        if (comp->insideBounds(x, y))
            return false;
    }

    if (!RUIComponent::insideBounds(x, y))
        return false;

    if (!(
            this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->viewport.w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->viewport.h))
        return false;

    return true;
}

void Frame::setBg(Color bg)
{
    this->bg = bg;
    this->fill(bg);
}

void Frame::setPosition(int x, int y)
{
    RUIComponent::setPosition(x, y);

    for (RUIComponent *comp : this->members)
    {
        Point compPos = comp->getPosition();
        comp->setAbsPosition(x + compPos.x, y + compPos.y);
        comp->setPosition(compPos.x, compPos.y);
    }

    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

void Frame::setOverflowX(bool ovf)
{
    this->overflowX = ovf;
}

void Frame::setOverflowY(bool ovf)
{
    this->overflowY = ovf;
}

void Frame::setViewport(int x, int y, int w, int h)
{
    this->viewport = {x, y, w, h};

    for (RUIComponent *comp : this->members)
    {
        comp->parviewport = {x, y, w, h};
    }
}

void Frame::add(RUIComponent *component)
{
    this->members.push_back(component);
    component->parviewport = this->viewport;
}

void Frame::del(RUIComponent *component)
{
    for (int i = 0; i < this->members.size(); i++)
    {
        if (this->members.at(i) == component)
            this->members.erase(this->members.begin() + i);
    }
}

vector<RUIComponent*> Frame::getMembers()
{
    return this->members;
}

SDL_Surface *Frame::getSDLSurface()
{
    return this->baseSurface;
}

void Frame::update()
{
    if (this->movingScrollBarV)
    {
        if (0 <= this->viewport.y + this->scrollBarMovingDistY && this->viewport.y + this->scrollBarMovingDistY <= this->getSize().h - this->viewport.h)
        {
            this->setViewport(this->viewport.x, this->viewport.y + this->scrollBarMovingDistY, this->viewport.w, this->viewport.h);
        }

        this->scrollBarMovingDistY = 0;
    }

    if (this->movingScrollBarH)
    {
        if (0 <= this->viewport.x + this->scrollBarMovingDistX && this->viewport.x + this->scrollBarMovingDistX <= this->getSize().w - this->viewport.w)
        {
            this->setViewport(this->viewport.x + this->scrollBarMovingDistX, this->viewport.y, this->viewport.w, this->viewport.h);
        }

        this->scrollBarMovingDistX = 0;
    }

    if (this->isOverflowY())
        this->drawVScrollBar();

    if (this->isOverflowX())
        this->drawHScrollBar();
}

void Frame::drawVScrollBar()
{
    int height = this->getSize().h;

    // draw scroll bar container
    drawLineToSurface(this->baseSurface,
                      this->viewport.w + this->viewport.x - this->DEFAULT_SCROLL_BAR_WIDTH, 0,
                      this->viewport.w + this->viewport.x - this->DEFAULT_SCROLL_BAR_WIDTH, this->viewport.h + this->viewport.y,
                      SDL_MapRGBA(this->baseSurface->format, BLACK));

    float ratio = (float)this->viewport.y / height;
    int thumbHeight = ((float)(this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0)) / height) * viewport.h;
    SDL_Surface *c = SDL_CreateRGBSurfaceWithFormat(0, this->DEFAULT_SCROLL_BAR_WIDTH, thumbHeight, this->baseSurface->format->BitsPerPixel, this->baseSurface->format->format);
    SDL_FillRect(c, NULL, SDL_MapRGBA(this->baseSurface->format, BLACK));

    SDL_Rect r = {
        this->viewport.w + this->viewport.x - c->w,
        (int)ceil((height / ((float)(this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))) + 1) * ratio * (this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))),
        c->w,
        c->h};

    SDL_BlitSurface(c, NULL, this->baseSurface, &r);
    SDL_FreeSurface(c);
}

void Frame::drawHScrollBar()
{
    int width = this->getSize().w;

    // draw scroll bar container
    drawLineToSurface(this->baseSurface,
                      0, this->viewport.h + this->viewport.y - this->DEFAULT_SCROLL_BAR_WIDTH,
                      width, this->viewport.h + this->viewport.y - this->DEFAULT_SCROLL_BAR_WIDTH,
                      SDL_MapRGBA(this->baseSurface->format, BLACK));

    // draw scroll thumb
    float ratio = (float)this->viewport.x / width;
    int thumbWidth = ((float)(this->viewport.w - (this->overflowY ? this->DEFAULT_SCROLL_BAR_WIDTH : 0)) / width) * viewport.w;
    SDL_Surface *c = SDL_CreateRGBSurfaceWithFormat(0, thumbWidth, this->DEFAULT_SCROLL_BAR_WIDTH, this->baseSurface->format->BitsPerPixel, this->baseSurface->format->format);
    SDL_FillRect(c, NULL, SDL_MapRGBA(this->baseSurface->format, BLACK));

    SDL_Rect r = {
        (int)ceil((width / ((float)(this->viewport.w - (this->overflowY ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))) + 1) * ratio * (this->viewport.w - (this->overflowY ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))),
        this->viewport.h + this->viewport.y - c->h,
        c->w,
        c->h};

    SDL_BlitSurface(c, NULL, this->baseSurface, &r);
    SDL_FreeSurface(c);
}

Frame::~Frame()
{
}

void Frame::onMousePressed(Event e)
{
    RUIComponent::onMousePressed(e);

    if (
        this->getAbsPosition().x + this->viewport.w - this->DEFAULT_SCROLL_BAR_WIDTH <= e.button.x && e.button.x <= this->getAbsPosition().x +  this->viewport.w
        && this->getAbsPosition().y <= e.button.y && e.button.y <= this->getAbsPosition().y + this->viewport.h - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0))
    {
        this->movingScrollBarV = true;
        this->scrollBarMovingStartY = e.button.y;
        this->scrollBarMovingDistY = 0;
    }
    else if (
        this->getAbsPosition().x <= e.button.x && e.button.x <= this->getAbsPosition().x + this->viewport.w - (this->overflowX ? this->DEFAULT_SCROLL_BAR_WIDTH : 0)
        && this->getAbsPosition().y + this->viewport.h - this->DEFAULT_SCROLL_BAR_WIDTH <= e.button.y && e.button.y <= this->getAbsPosition().y +  this->viewport.h)
    {
        this->movingScrollBarH = true;
        this->scrollBarMovingStartX = e.button.x;
        this->scrollBarMovingDistX = 0;
    }
}

void Frame::onMouseReleased(Event e)
{
    RUIComponent::onMouseReleased(e);

    this->movingScrollBarV = false;
    this->movingScrollBarH = false;
}

void Frame::onMouseMotion(Event e)
{
    RUIComponent::onMouseMotion(e);

    if (this->movingScrollBarV)
    {
        this->scrollBarMovingDistY = e.button.y - this->scrollBarMovingStartY;
        this->scrollBarMovingStartY = e.button.y;
    }

    if (this->movingScrollBarH)
    {
        this->scrollBarMovingDistX = e.button.x - this->scrollBarMovingStartX;
        this->scrollBarMovingStartX = e.button.x;
    }
}

void Frame::onMouseLeaved(Event e)
{
	RUIComponent::onMouseLeaved(e);

    this->movingScrollBarV = false;
    this->movingScrollBarH = false;
}
