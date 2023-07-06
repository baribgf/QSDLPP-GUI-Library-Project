#pragma once

#include "label.hpp"

class Button : public Label
{
protected:

    SDL_Color DEFAULT_HOVER_BG;
    SDL_Color DEFAULT_HOVER_FG;

    void onClick(Event e) override;
    void onMouseEntered(Event e) override;
    void onMouseLeaved(Event e) override;

public:
    Button(RelativeUIComponent *parent, string text, int width, int height);

    SDL_Color getBorderColor();
    void setBorderColor(SDL_Color color);
};
