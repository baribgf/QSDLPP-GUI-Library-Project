#include "../QSDL++/Application"
#include "../QSDL++/Window"
#include "../QSDL++/UIComponents/Frame"
#include "../QSDL++/UIComponents/Button"

int main(int argc, char const *argv[])
{
    Application app(argc, argv);
    Window win("Example", 600, 400);
    Frame f(nullptr, 560, 360);
    f.setBordersVisible(true);
    f.setPosition(20, 20);

    win.addFrame(&f);
    app.setWindow(&win);
    app.exec();
    return 0;
}
