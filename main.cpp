#include "headers/application.hpp"
#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/textinput.hpp"

int main(int argc, char const *argv[])
{
	Application app(argc, argv);

	Window window("Main Window", 800, 500);

	Frame frame(nullptr, 800, 500);
	frame.setBg({BLUE});

	TextInput t(&frame, 200, 30);
	t.setPosition(100, 100);
	
	frame.add(&t);

	window.addFrame(&frame);

	app.setWindow(&window);
	app.exec();

	debug("Good bye! ..");
	return 0;
}
