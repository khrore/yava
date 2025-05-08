#include "app/triangle.hxx"

#include <cstdlib>
#include <exception>
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