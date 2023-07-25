#include "../QSDL++/Application"
#include "../QSDL++/Window"
#include "../QSDL++/UIComponents/Frame"
#include "../QSDL++/UIComponents/Button"

int main(int argc, char const *argv[])
{
    Application app(argc, argv);
    Window win("Example", 600, 400);
    Frame f1(nullptr, 580, 380);
    Frame f2(&f1, 500, 300);
    f1.setBg({BLUE});
    f1.setPosition(10, 10);
    f2.setBg({RED});
    f2.setPosition(20, 20);

    Button btn1(&f2, "Click Me", 120, 30);
    btn1.setPosition(10, 10);

    f2.add(&btn1);

    f1.add(&f2);

    win.addFrame(&f1);
    app.setWindow(&win);
    app.exec();
    return 0;
}
