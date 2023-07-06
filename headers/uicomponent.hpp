#pragma once

#include "common.hpp"
#include "event.hpp"

class UIComponent
{
protected:
    int x, y;
    SDL_Surface *baseSurface;
    SDL_Color fillColor;
    bool mouseEntered;
    bool mousePressed;
    bool mouseReleased;
    bool visible;
    SDL_Cursor *DEFAULT_CURSOR, *HAND_CURSOR;

    virtual void onClick(Event e);
    virtual void onMousePressed(Event e);
    virtual void onMouseReleased(Event e);
    virtual void onMouseEntered(Event e);
    virtual void onMouseLeaved(Event e);

    void (*onClickHandlerPtr)(Event);
    void (*onMousePressedHandlerPtr)(Event);
    void (*onMouseReleasedHandlerPtr)(Event);
    void (*onMouseEnteredHandlerPtr)(Event);
    void (*onMouseLeavedHandlerPtr)(Event);

    virtual bool insideBounds(int x, int y);

public:
    UIComponent(Uint16 width, Uint16 height, SDL_Color fillColor = {WHITE});

    bool isVisible();
    SDL_Size getSize();
    virtual SDL_Point getAbsPosition();
    
    SDL_Color getFillColor();
    SDL_Surface *getSDLSurface();

    virtual void setSize(Uint16 width, Uint16 height);
    void setVisible(bool visible);
    virtual void setAbsPosition(int x, int y);

    /**
     * Fill the entire surface.
     * @param color filling color
     */
    void fill(SDL_Color color);

    /**
     * Invoke the component events.
     * @param event
     */
    virtual void invokeEvents(SDL_Event event);

    void setOnClickHandler(void (*handler)(Event));
    void setOnMousePressedHandler(void (*handler)(Event));
    void setOnMouseReleasedHandler(void (*handler)(Event));
    void setOnMouseEnteredHandler(void (*handler)(Event));
    void setOnMouseLeavedHandler(void (*handler)(Event));

    ~UIComponent();
};
