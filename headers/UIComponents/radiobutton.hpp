#pragma once

#include "checkbox.hpp"

class RadioButton : public CheckBox
{
protected:
    void onClick(Event e) override;
    vector<RadioButton *> otherMembers;
    
public:
    RadioButton(RUIComponent *parent, std::string text, int width, int height);
    ~RadioButton();

    void link(RadioButton *radiobutton);
    void unlink(RadioButton *radiobutton);
};
