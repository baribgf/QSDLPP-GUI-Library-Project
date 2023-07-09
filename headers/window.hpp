#pragma once

#include "common.hpp"
#include "uicomponent.hpp"
#include "UIComponents/frame.hpp"

class Window
{
private:
    void mainloop();
    void handleEvents();
    void handleFocusing();
    void renderComponent(UIComponent *uicomponent);
    void renderFrame(Frame *frame);
    bool running;
    bool thereWasPendingEvent;
    SDL_Window *baseWindow;
    SDL_Renderer *mainRenderer;
    SDL_Event event;
    vector<Frame*> frames;

    vector<RUIComponent *> haveFocus;
    Uint64 maxFocusTimeID = 0;

protected:
    Uint16 FPS, frameDelay;
    RUIComponent *focusedComponent;
    virtual void onWindowResized(Event e);

public:
    Window(string title, int width, int height, bool fullscreen = false, bool centered = true);

    void setSize(Uint16 width, Uint16 height);
    void setPosition(int x, int y);
    void setFPS(Uint16 fps);

    Dimension getSize();
    Point getPosition();
    Uint16 getFPS();

    void setCentered();

    void addFrame(Frame *frame);
    void delFrame(Frame *frame);

    /**
     * Start the window application.
     */
    void exec();

    ~Window();
};
