#pragma once

#include "label.hpp"

class TextInput : public Label
{
public:
    TextInput(RelativeUIComponent *parent, int width, int height);
    ~TextInput();
};
