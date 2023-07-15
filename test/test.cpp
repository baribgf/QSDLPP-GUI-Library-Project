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

    Button btn(&f, "Button 1", 150, 30);
    btn.setPosition(100, 100);
    btn.setOnClickHandler(
        [](Event e, void *data)
        {
            Event event;
            event.type = EventType::QUIT;
            event.push();
        }, nullptr);

    f.add(&btn);

    win.addFrame(&f);
    app.setWindow(&win);
    app.exec();
    return 0;
}
