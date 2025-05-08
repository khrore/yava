#include "triangle.hxx"

namespace App
{
Triangle::Triangle(): vulkan(window) {
}

void Triangle::run()
{
	mainLoop();
}

void Triangle::mainLoop()
{
	while (!glfwWindowShouldClose(&window.get()))
	{
		glfwPollEvents();
	}
}
}        // namespace App