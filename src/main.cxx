#include "app/triangle.hxx"

#include <iostream>

int main()
{
	App::Triangle app;
	try
	{
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}