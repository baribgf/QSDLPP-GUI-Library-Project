#include "../../headers/UIComponents/frame.hpp"

Frame::Frame(RUIComponent *parent, int width, int height) : RUIComponent(parent, width, height)
{
}

Color Frame::getBg()
{
    return this->bg;
}

RUIComponent *Frame::getMemberAt(int index)
{
    return this->members.at(index);
}

int Frame::getSizeOfMembers()
{
    return this->members.size();
}

bool Frame::insideBounds(int x, int y)
{
    for (RUIComponent *comp : this->members)
    {
        if (comp->insideBounds(x, y))
            return false;
    }

    if (this->getAbsPosition().x <= x && x <= this->getAbsPosition().x + this->getSize().w && this->getAbsPosition().y <= y && y <= this->getAbsPosition().y + this->getSize().h)
    {
        if (this->getParent())
        {
            RUIComponent *parent = dynamic_cast<RUIComponent *>(this->getParent());
            if (parent)
                return (parent->visibleArea.x <= x && x <= parent->visibleArea.x + parent->visibleArea.w && parent->visibleArea.y <= y && y <= parent->visibleArea.y + parent->visibleArea.h);
        }
        return true;
    }

    return false;
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

void Frame::add(RUIComponent *component)
{
    this->members.push_back(component);
}

void Frame::del(RUIComponent *component)
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
