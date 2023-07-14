#pragma once

#include "common.hpp"
#include "event.hpp"

class UIComponent
{
protected:
    int x, y;
    SDL_Surface *baseSurface;
    Color fillColor;
    bool mouseEntered;
    bool mousePressed;
    bool mouseReleased;
    bool visible;

    void *clickHandlerData;
    void *pressHandlerData;
    void *releaseHandlerData;
    void *enterHandlerData;
    void *leaveHandlerData;
    void *motionHandlerData;

    virtual void onClick(Event e);
    virtual void onMousePressed(Event e);
    virtual void onMouseReleased(Event e);
    virtual void onMouseEntered(Event e);
    virtual void onMouseLeaved(Event e);
    virtual void onMouseMotion(Event e);

    void (*onClickHandlerPtr)(Event, void*);
    void (*onMousePressedHandlerPtr)(Event, void*);
    void (*onMouseReleasedHandlerPtr)(Event, void*);
    void (*onMouseEnteredHandlerPtr)(Event, void*);
    void (*onMouseLeavedHandlerPtr)(Event, void*);
    void (*onMouseMotionHandlerPtr)(Event, void*);

    virtual bool insideBounds(int x, int y);

public:
    UIComponent(Uint16 width, Uint16 height);

    bool isVisible();
    Dimension getSize();
    virtual Point getAbsPosition();
    
    Color getFillColor();
    virtual SDL_Surface *getSDLSurface();

    virtual void setSize(Uint16 width, Uint16 height);
    void setVisible(bool visible);
    virtual void setAbsPosition(int x, int y);

    /**
     * Fill the entire surface.
     * @param color filling color
     */
    void fill(Color color);

    /**
     * Invoke the component events.
     * @param event
     */
    virtual void invokeEvents(Event event);

    void setOnClickHandler(void (*handler)(Event e, void*), void *data);
    void setOnMousePressedHandler(void (*handler)(Event e, void*), void *data);
    void setOnMouseReleasedHandler(void (*handler)(Event e, void*), void *data);
    void setOnMouseEnteredHandler(void (*handler)(Event e, void*), void *data);
    void setOnMouseLeavedHandler(void (*handler)(Event e, void*), void *data);
    void setOnMouseMotionHandler(void (*handler)(Event e, void*), void *data);

    virtual ~UIComponent();
};
