#pragma once

#include "../reluicomp.hpp"

class Label : public RUIComponent
{
protected:
    bool autoImgSize;
    bool bordersVisible;
    Uint8 textSize;
    int textX, textY;
    int imgX, imgY;
    Uint16 textW, textH;
    Uint16 imgW, imgH;
    string text;
    string imgPath;
    string FONT_PATH;
    Align textAlign;
    Align imgAlign;
    Color fg;
    Color bordersColor;
    Color DEFAULT_BG;
    Color DEFAULT_FG;
    Color DEFAULT_BORDERS_COLOR;

    virtual void update();
    void drawBorders();
    void drawText();
    void drawImage();
    void alignImage();

public:
    Label(RUIComponent *parent, string text, int width, int height);

    string getText();
    Color getFg();
    Color getBg();
    Uint8 getTextSize();
    Align getAlign();
    Color getBordersColor();
    Dimension getImageSize();

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
    void setTextSize(Uint8 p);

    /**
     * Align text amoung component bounds.
     * @param a align type: 'CENTER', 'NORTH', 'SOUTH_EAST', .. etc.
     */
    virtual void setTextAlign(Align a);
    void setBordersVisible(bool visible);
    void setBordersColor(Color color);
    void setImage(string imgPath, Align align = Align::NORTH_WEST);
    void setImageSize(Uint32 width, Uint32 height);
    void setImageSizeAuto(bool _auto);
};
