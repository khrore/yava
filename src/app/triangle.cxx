#include "triangle.hxx"
#include "app/window/window.hxx"

namespace App
{
Triangle::Triangle() : vulkan(window)
{}

void Triangle::run()
{
	mainLoop();
}

void Triangle::mainLoop()
{
	while (!glfwWindowShouldClose(&window.get()))
	{
		glfwPollEvents();
		vulkan.drawFrame();
	}
	vulkan.endDraw();
}
}        // namespace App