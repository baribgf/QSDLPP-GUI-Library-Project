#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/button.hpp"

int main(int argc, char const *argv[])
{
    Window window("Main Window", 800, 500);
    
    Frame frame(nullptr, 800, 500);
    frame.setBg({BLUE});
    frame.setViewport(0, 0, 0.75 * frame.getSize().w, 0.75 * frame.getSize().h);

    Button btn1(&frame, "Button 1", 150, 30);
    Button btn2(&frame, "Button 2", 150, 30);
    Button btn3(&frame, "Button 3", 150, 30);

    btn1.setPosition(20, 20);
    btn2.setPosition(200, 200);
    btn3.setPosition(600, 360);

    frame.add(&btn1);
    frame.add(&btn2);
    frame.add(&btn3);

    window.addFrame(&frame);

    window.exec();

    debug("Good bye! ..");
    return 0;
}
