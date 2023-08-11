#include "../QSDL++/Application"
#include "../QSDL++/Window"
#include "../QSDL++/UIComponents/Frame"
#include "../QSDL++/UIComponents/Button"

int main(int argc, char const *argv[])
{
    Application app(argc, argv);
    Window win("Example", 600, 400);
    Frame f(nullptr, 580, 380);
    win.addFrame(&f);
    app.setWindow(&win);
    app.exec();
    return 0;
}
