#pragma once

#include "label.hpp"

class TextInput : public Label
{
protected:
    bool uppercase;
    Uint8 cursorIndex;

    void onKeyPressed(Event e) override;
    void onKeyReleased(Event e) override;

    void drawCursor();
    void update() override;
    void setFocus(bool focus) override;
    
public:
    TextInput(RUIComponent *parent, int width, int height);
    ~TextInput();
};
