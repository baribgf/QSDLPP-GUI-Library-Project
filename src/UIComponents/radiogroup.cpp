#include "../../headers/UIComponents/radiogroup.hpp"

RadioGroup::RadioGroup()
{
}

RadioGroup::~RadioGroup()
{
}

void RadioGroup::add(RadioButton *radioButton)
{
    for (RadioButton *rb : this->radioMembers)
    {
        if (rb != radioButton)
        {
            radioButton->link(rb);
            rb->link(radioButton);
        }
        else
            return;
    }

    this->radioMembers.push_back(radioButton);
}

void RadioGroup::del(RadioButton *radioButton)
{
    for (int i = 0; i < this->radioMembers.size(); i++)
    {
        if (this->radioMembers.at(i) == radioButton)
            this->radioMembers.erase(this->radioMembers.begin() + i);
    }
}

vector<RadioButton *> RadioGroup::getMembers()
{
    return this->radioMembers;
}
