#pragma once

#include "label.hpp"

class Checkbox : public Label
{
public:
    Checkbox(RUIComponent *parent, string text, int width, int height);
    ~Checkbox();
};
