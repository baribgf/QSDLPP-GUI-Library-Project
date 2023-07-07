#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/button.hpp"

int main(int argc, char const *argv[])
{
    Window window("Main Window", 800, 500);
    Frame frame(nullptr, window.getSize().w, window.getSize().h);
    frame.setBg({BLUE});

    window.addFrame(&frame);

    Button btn(&frame, "Click Me!", 120, 30);
    btn.setPosition(110, 110);

    btn.setOnClickHandler(
        [](Event e)
        {
            e.code = 2004;
            e.push();
        }
    );
    
    frame.addComponent(&btn);

    window.exec();

    debug("Exiting ..");
    return 0;
}
