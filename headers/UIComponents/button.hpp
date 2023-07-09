#pragma once

#include "label.hpp"

class Button : public Label
{
protected:
    Color DEFAULT_HOVER_BG;
    Color DEFAULT_HOVER_FG;

    void onClick(Event e) override;
    void onMouseEntered(Event e) override;
    void onMouseLeaved(Event e) override;
    void onKeyReleased(Event e) override;

public:
    Button(RUIComponent *parent, string text, int width, int height);

    Color getBorderColor();
    void setBorderColor(Color color);

    void setFocus(bool focus) override;
};
