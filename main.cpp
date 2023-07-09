#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/checkbox.hpp"

int main(int argc, char const *argv[])
{
    Window window("Main Window", 800, 500);
    Frame frame(nullptr, window.getSize().w, window.getSize().h);
    frame.setBg({BLUE});
    
    Checkbox cb(&frame, "Check Me!", 120, 30);
    cb.setPosition(100, 100);

    frame.add(&cb);

    window.addFrame(&frame);
    window.exec();

    debug("Good bye! ..");
    return 0;
}
