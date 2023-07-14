#include "../QSDL++/Application"
#include "../QSDL++/Window"

int main(int argc, char const *argv[])
{
	Application app(argc, argv);
	Window window("Main Window", 800, 500);

	app.setWindow(&window);
	app.exec();

	debug("Good bye! ..");
	return 0;
}
