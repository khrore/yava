#include "app.hxx"
#include "app/window/window.hxx"

namespace App
{
App::App() : vulkan(window)
{}

void App::run()
{
	mainLoop();
}

void App::mainLoop()
{
	while (!glfwWindowShouldClose(&window.get()))
	{
		glfwPollEvents();
		vulkan.drawFrame();
	}
	vulkan.endDraw();
}
}        // namespace App