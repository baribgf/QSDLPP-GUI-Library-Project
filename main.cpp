#include "headers/application.hpp"
#include "headers/window.hpp"
#include "headers/UIComponents/frame.hpp"
#include "headers/UIComponents/canvas.hpp"

int main(int argc, char const *argv[])
{
	Application app(argc, argv);
	Window window("Main Window", 800, 500, WindowFlags::WINDOW_BORDERLESS);

	app.setWindow(&window);
	app.exec();

	debug("Good bye! ..");
	return 0;
}
