#pragma once

#include "../common.hpp"
#include "../reluicomp.hpp"

class Canvas : public RUIComponent
{
protected:
    Color drawColor;

public:
    Canvas(UIComponent *parent, Uint16 width, Uint16 height);
    ~Canvas();

    Color getDrawColor();

    void drawLine(int x1, int y1, int x2, int y2);
    void drawArc(int x, int y, int radius, int startAngle, int endAngle);
    void drawFilledArc(int x, int y, int radius, int startAngle, int endAngle);
    void drawRect(int x, int y, int width, int height);
    void drawPoint(int x, int y);

    void setDrawColor(Color color);
};
