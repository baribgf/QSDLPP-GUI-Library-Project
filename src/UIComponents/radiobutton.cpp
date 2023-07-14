#include "../../QSDL++/UIComponents/RadioButton"

RadioButton::RadioButton(RUIComponent *parent, std::string text, int width, int height) : CheckBox(parent, text, width, height)
{
}

RadioButton::~RadioButton()
{
}

void RadioButton::onClick(Event e)
{
    Label::onClick(e);

    int posX = this->getAbsPosition().x + this->cbX;
    int posY = this->getAbsPosition().y + this->cbY;
    int sizeW = this->cbW;
    int sizeH = this->cbH;

    if (posX <= e.button.x && e.button.x <= posX + sizeW && posY <= e.button.y && e.button.y <= posY + sizeH)
    {
        this->setChecked(true);

        for (RadioButton *rb : this->otherMembers)
        {
            rb->setChecked(false);
        }
    }
}

void RadioButton::link(RadioButton *radiobutton)
{
    if (radiobutton != this)
        this->otherMembers.push_back(radiobutton);
}

void RadioButton::unlink(RadioButton *radiobutton)
{
    for (int i = 0; i < this->otherMembers.size(); i++)
    {
        if (this->otherMembers.at(i) == radiobutton)
            this->otherMembers.erase(this->otherMembers.begin() + i);
    }
}
