#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/button.hpp"

int main(int argc, char const *argv[])
{
    Window window("Main Window", 800, 500);
    Frame frame(nullptr, window.getSize().w, window.getSize().h);

    window.addFrame(&frame);

    Button btn(&frame, "Click Me!", 120, 30);
    
    frame.addComponent(&btn);

    window.exec();
    return 0;
}
