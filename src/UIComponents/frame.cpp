#include "../../headers/UIComponents/frame.hpp"

Frame::Frame(RelativeUIComponent *parent, int width, int height) : RelativeUIComponent(parent, width, height)
{
}

Color Frame::getBg()
{
    return this->bg;
}

RelativeUIComponent *Frame::getMemberAt(int index)
{
    return this->members.at(index);
}

int Frame::getSizeOfMembers()
{
    return this->members.size();
}

void Frame::setBg(Color bg)
{
    this->bg = bg;
    this->fill(bg);
}

void Frame::setPosition(int x, int y)
{
    this->relX = x;
    this->relY = y;

    if (this->parent != nullptr)
    {
        Point parPos = this->parent->getAbsPosition();
        x += parPos.x;
        y += parPos.y;
    }
    
    this->setAbsPosition(x, y);

    for (RelativeUIComponent *comp : this->members)
    {
        Point compPos = comp->getPosition();
        comp->setAbsPosition(x + compPos.x, y + compPos.y);
        comp->setPosition(compPos.x, compPos.y);
    }

    this->updateVisibleArea(this->getAbsPosition().x, this->getAbsPosition().y, this->getSize().w, this->getSize().h);
}

void Frame::addComponent(RelativeUIComponent *component)
{
    this->members.push_back(component);
}

void Frame::delComponent(RelativeUIComponent *component)
{
    for (int i = 0; i < this->members.size(); i++)
    {
        if (this->members.at(i) == component)
            this->members.erase(this->members.begin() + i);
    }
}

Frame::~Frame()
{
}
