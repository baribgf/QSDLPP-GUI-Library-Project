#include "../QSDL++/Application"
#include "../QSDL++/Window"
#include "../QSDL++/UIComponents/Frame"
#include "../QSDL++/UIComponents/Button"

int main(int argc, char const* argv[])
{
    Application app(argc, argv);
    Window win("Example", 600, 400);
    Frame f(nullptr, 600, 400);
    f.setBg({ BLUE });

    Button btn1(&f, "Button 1", 150, 30);
    Button btn2(&f, "Button 2", 150, 30);
    btn1.setPosition(100, 100);
    btn2.setPosition(253, 100);

    f.add(&btn1);
    f.add(&btn2);

    win.addFrame(&f);
    app.setWindow(&win);
    app.exec();
    return 0;
}
