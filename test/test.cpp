#include "../QSDL++/Application"
#include "../QSDL++/Window"
#include "../QSDL++/UIComponents/Frame"
#include "../QSDL++/UIComponents/Button"

int main(int argc, char const *argv[])
{
    Application app(argc, argv);
    Window win("Example", 600, 400);
    Frame f(nullptr, 580, 380);
    f.setBg({BLUE});
    f.setPosition(10, 10);

    Button btn(&f, "Click Me", 120, 30);
    btn.setPosition(10, 10);

    f.add(&btn);

    win.addFrame(&f);
    app.setWindow(&win);
    app.exec();
    return 0;
}
