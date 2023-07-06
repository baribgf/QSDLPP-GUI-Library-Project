#pragma once

#include "uicomponent.hpp"

class RelativeUIComponent : public UIComponent
{
protected:
    int relX;
    int relY;
    UIComponent *parent;

public:
    RelativeUIComponent(UIComponent *parent, Uint16 width, Uint16 height, SDL_Color fillColor = {WHITE});
    ~RelativeUIComponent();

    SDL_Rect visibleArea;

    void updateVisibleArea(int x, int y, int width, int height);

    /**
     * Get relative position
     */
    SDL_Point getPosition();

    /**
     * Set relative position
     */
    virtual void setPosition(int x, int y);

    void setSize(Uint16 width, Uint16 height) override;

    bool insideBounds(int x, int y) override;

    UIComponent *getParent();
};
