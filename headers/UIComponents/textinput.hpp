#pragma once

#include "label.hpp"

class TextInput : public Label
{
protected:
    int shmnt;
    bool inspecting;
    bool uppercase;
    bool ctrl;
    int cursorIndex;
    string textToRender;

    void onKeyPressed(Event e) override;
    void onKeyReleased(Event e) override;

    void drawCursor();
    void update() override;
    void drawText() override;
    void setFocus(bool focus) override;
    
public:
    TextInput(RUIComponent *parent, int width, int height);
    ~TextInput();
};
