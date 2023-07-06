#include "headers/window.hpp"

int main(int argc, char const *argv[])
{
    // Window window("Main Window", 800, 500);
    // window.exec();
    // return 0;
    Window win("My Window", 600, 400);
    
    Frame frame(nullptr, 400, 200);
    frame.setBg(toSDLColor(RED));
    frame.setPosition(
        (win.getSize().w - frame.getSize().w) / 2,
        (win.getSize().h - frame.getSize().h) / 2
    );

    Button btn(&frame, "Click Me!", 200, 40);
    btn.setPtSize(18);
    btn.setPosition(
        (frame.getSize().w - btn.getSize().w) / 2,
        (frame.getSize().h - btn.getSize().h) / 2
    );

    btn.setOnClickHandler(
        [](Event e)
        {
            debug("Hello, World!");
        }
    );

    frame.addComponent(&btn);

    win.addFrame(&frame);
    win.exec();
    return 0;
}
