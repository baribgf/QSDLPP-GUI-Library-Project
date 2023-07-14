#include <QSDL++/Application>
#include <QSDL++/Window>
#include <QSDL++/Thread>
#include <QSDL++/UIComponents/Frame>
#include <QSDL++/UIComponents/Label>
#include <QSDL++/UIComponents/Button>

int main(int argc, char const *argv[])
{
    Application app(argc, argv);

    Window window("My Window", 600, 400);
    vector<Frame*> frames;

    Frame f1(nullptr, 600, 400);
    Frame f2(nullptr, 600, 400);
    Frame f3(nullptr, 600, 400);
    f1.setBg({RED});
    f2.setBg({GREEN});
    f3.setBg({BLUE});
    f2.setVisible(false);
    f3.setVisible(false);

    frames.push_back(&f1);
    frames.push_back(&f2);
    frames.push_back(&f3);

    Label lbl1(&f1, "This is Label 1", 200, 30);
    Label lbl2(&f2, "This is Label 2", 200, 30);
    Label lbl3(&f3, "This is Label 3", 200, 30);
    lbl1.setPosition(200, 185);
    lbl2.setPosition(200, 185);
    lbl3.setPosition(200, 185);
    lbl1.setTextAlign(Align::CENTER);
    lbl2.setTextAlign(Align::CENTER);
    lbl3.setTextAlign(Align::CENTER);

    Button btn1(&f1, "Next 2", 100, 30);
    Button btn2(&f2, "Next 3", 100, 30);
    Button btn3(&f3, "Next 1", 100, 30);
    btn1.setPosition(10, 360);
    btn2.setPosition(10, 360);
    btn3.setPosition(10, 360);

    btn1.setOnClickHandler(
        [](Event e, void *data)
        {
            vector<Frame*>* frames = (vector<Frame*>*) data;
            (*frames).at(0)->setVisible(false);
            (*frames).at(1)->setVisible(true);
            (*frames).at(2)->setVisible(false);
        }, &frames);

    btn2.setOnClickHandler(
        [](Event e, void *data)
        {
            vector<Frame*>* frames = (vector<Frame*>*) data;
            (*frames).at(0)->setVisible(false);
            (*frames).at(1)->setVisible(false);
            (*frames).at(2)->setVisible(true);
        }, &frames);

    btn3.setOnClickHandler(
        [](Event e, void *data)
        {
            vector<Frame*>* frames = (vector<Frame*>*) data;
            (*frames).at(2)->setVisible(false);
            (*frames).at(0)->setVisible(true);
            (*frames).at(1)->setVisible(false);
        }, &frames);

    f1.add(&lbl1);
    f2.add(&lbl2);
    f3.add(&lbl3);
    f1.add(&btn1);
    f2.add(&btn2);
    f3.add(&btn3);

    window.addFrame(&f1);
    window.addFrame(&f2);
    window.addFrame(&f3);

    app.setWindow(&window);

    debug("Starting ..");
    app.exec();
    debug("Good bye! ..");

    return 0;
}
