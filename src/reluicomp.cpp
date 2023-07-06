#include "../headers/reluicomp.hpp"

RelativeUIComponent::RelativeUIComponent(RelativeUIComponent *parent, Uint16 width, Uint16 height, SDL_Color fillColor) : UIComponent(width, height, fillColor)
{
    this->parent = parent;
    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

SDL_Point RelativeUIComponent::getPosition()
{
    return {this->relX, this->relY};
}

RelativeUIComponent *RelativeUIComponent::getParent()
{
    return this->parent;
}

bool RelativeUIComponent::insideBounds(int x, int y)
{
    if (this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->getSize().w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->getSize().h)
    {
        if (this->getParent())
            return (this->getParent()->visibleArea.x <= x && x <= this->getParent()->visibleArea.x + this->getParent()->visibleArea.w && this->getParent()->visibleArea.y <= y && y <= this->getParent()->visibleArea.y + this->getParent()->visibleArea.h);
       return true;
    }
    return false;
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
        SDL_Point parPos = this->parent->getAbsPosition();
        x += parPos.x;
        y += parPos.y;
    }

    this->setAbsPosition(x, y);

    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

RelativeUIComponent::~RelativeUIComponent()
{
}
