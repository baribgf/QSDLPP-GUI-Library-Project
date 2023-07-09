#pragma once

#include "../common.hpp"
#include "radiobutton.hpp"

class RadioGroup
{
protected:
    vector<RadioButton *> radioMembers;

public:
    RadioGroup();
    ~RadioGroup();


    vector<RadioButton *> getMembers();
    void add(RadioButton *radioButton);
    void del(RadioButton *radioButton);
};
