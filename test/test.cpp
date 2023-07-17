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

    Label lbl(&f, "Hello, World!", 180, 30);
    lbl.setTextAlign(Align::CENTER);
    lbl.setPosition(100, 100);
    lbl.setRightBorderVisible(true);

    f.add(&lbl);

    win.addFrame(&f);
    app.setWindow(&win);
    app.exec();
    return 0;
}
