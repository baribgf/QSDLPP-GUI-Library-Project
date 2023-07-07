#pragma once

#include "uicomponent.hpp"

class RelativeUIComponent : public UIComponent
{
protected:
    int relX;
    int relY;
    bool focus;
    UIComponent *parent;

    void onKeyPressed(Event e);
    void onKeyReleased(Event e);

    void (*onKeyPressedHandlerPtr)(Event);
    void (*onKeyReleasedHandlerPtr)(Event);

public:
    RelativeUIComponent(UIComponent *parent, Uint16 width, Uint16 height);
    ~RelativeUIComponent();

    SDL_Rect visibleArea;

    void updateVisibleArea(int x, int y, int width, int height);

    /**
     * Get relative position
     */
    Point getPosition();

    /**
     * Set relative position
     */

    bool insideBounds(int x, int y) override;
    bool hasFocus();
    UIComponent *getParent();

    virtual void setPosition(int x, int y);
    void setSize(Uint16 width, Uint16 height) override;
    void setFocus(bool focus);

    bool invokeEvents(Event event) override;

    void setOnKeyPressedHandler(void (*handler)(Event e));
    void setOnKeyReleasedHandler(void (*handler)(Event e));
};
