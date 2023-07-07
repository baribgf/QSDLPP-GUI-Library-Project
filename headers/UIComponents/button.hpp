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

public:
    Button(RelativeUIComponent *parent, string text, int width, int height);

    Color getBorderColor();
    void setBorderColor(Color color);
};
