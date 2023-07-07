#pragma once

#include "../reluicomp.hpp"

class Label : public RelativeUIComponent
{
protected:
    int textX;
    int textY;
    bool bordersVisible;
    Uint8 ptSize;
    Uint16 textW, textH;
    string text;
    string FONT_PATH;
    Align alignType;
    Color fg;
    Color borderColor;
    Color DEFAULT_BG;
    Color DEFAULT_FG;
    Color DEFAULT_BORDER_COLOR;

    void update(string text, Uint8 ptSize, Color fg);
    void updateBorders();

public:
    Label(RelativeUIComponent *parent, string text, int width, int height);

    string getText();
    Color getFg();
    Color getBg();
    Uint8 getPtSize();
    Align getAlign();

    void setText(string text);

    /**
     * Set the foreground or the text color.
     * @param fg color for foreground.
     */
    void setFg(Color fg);

    /**
     * Set the background color.
     * @param bg color for background.
     */
    void setBg(Color bg);
    void setPtSize(Uint8 p);

    /**
     * Align text amoung component bounds.
     * @param a align type: 'CENTER', 'NORTH', 'SOUTH_EAST', .. etc.
     */
    void setTextAlign(Align a);

    void setBordersVisible(bool visible);
};
