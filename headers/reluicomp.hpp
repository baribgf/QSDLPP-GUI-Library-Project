#pragma once

#include "uicomponent.hpp"

class RUIComponent : public UIComponent
{
protected:
    int relX;
    int relY;
    bool focus;
    UIComponent *parent;

    virtual void onKeyPressed(Event e);
    virtual void onKeyReleased(Event e);

    void (*onKeyPressedHandlerPtr)(Event);
    void (*onKeyReleasedHandlerPtr)(Event);

    void onClick(Event e) override;

public:
    RUIComponent(UIComponent *parent, Uint16 width, Uint16 height);
    virtual ~RUIComponent();

    SDL_Rect visibleArea;
    SDL_Rect parviewport;
    Uint64 focusTimeID;

    virtual bool insideBounds(int x, int y) override;

    void updateVisibleArea(int x, int y, int width, int height);

    /**
     * Get relative position
     */
    Point getPosition();

    /**
     * Set relative position
     */

    bool hasFocus();
    UIComponent *getParent();

    virtual void setPosition(int x, int y);
    void setSize(Uint16 width, Uint16 height) override;
    virtual void setFocus(bool focus);

    void invokeEvents(Event event) override;

    void setOnKeyPressedHandler(void (*handler)(Event e));
    void setOnKeyReleasedHandler(void (*handler)(Event e));
};
