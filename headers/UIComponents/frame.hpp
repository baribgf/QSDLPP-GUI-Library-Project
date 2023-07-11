#pragma once

#include "../../headers/reluicomp.hpp"

class Frame : public RUIComponent
{
protected:
    int DEFAULT_SCROLL_BAR_WIDTH;
    bool overflowX;
    bool overflowY;
    bool movingScrollBarV;
    bool movingScrollBarH;
    int scrollBarMovingDistY;
    int scrollBarMovingDistX;
    int scrollBarMovingStartY;
    int scrollBarMovingStartX;
    Color bg;
    vector<RUIComponent*> members;

    bool insideBounds(int x, int y) override;
    void onMousePressed(Event e) override;
    void onMouseReleased(Event e) override;
    void onMouseMotion(Event e) override;
    void onMouseLeaved(Event e) override;

    void drawVScrollBar();
    void drawHScrollBar();

public:
    SDL_Rect viewport;

    Frame(RUIComponent *parent, int width, int height);
    virtual ~Frame();

    Color getBg();
    vector<RUIComponent*> getMembers();
    bool isOverflowX();
    bool isOverflowY();

    void setBg(Color bg);
    void setPosition(int x, int y) override;
    void setViewport(int x, int y, int w, int h);
    void setOverflowX(bool ovf);
    void setOverflowY(bool ovf);
    
    void update();
    
    void add(RUIComponent *component);
    void del(RUIComponent *component);

    SDL_Surface *getSDLSurface() override;
};
