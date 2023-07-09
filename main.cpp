#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/radiobutton.hpp"
#include "headers/UIComponents/radiogroup.hpp"

int main(int argc, char const *argv[])
{
    Window window("Main Window", 800, 500);
    Frame frame(nullptr, window.getSize().w, window.getSize().h);
    frame.setBg({BLUE});
    
    RadioButton rb1(&frame, "Radio 1", 120, 30);
    RadioButton rb2(&frame, "Radio 2", 120, 30);
    RadioButton rb3(&frame, "Radio 3", 120, 30);

    rb1.setPosition(200, 100);
    rb2.setPosition(200, 200);
    rb3.setPosition(200, 300);

    RadioGroup radioGroup;
    radioGroup.add(&rb1);
    radioGroup.add(&rb2);
    radioGroup.add(&rb3);

    frame.add(&rb1);
    frame.add(&rb2);
    frame.add(&rb3);

    window.addFrame(&frame);
    window.exec();

    debug("Good bye! ..");
    return 0;
}
