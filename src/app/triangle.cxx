#include "triangle.hxx"
#include "app/window/window.hxx"

namespace App
{
Triangle::Triangle()
{
    window.init(isFramebufferResized);
    vulkan.init(window, isFramebufferResized);
}

Triangle::~Triangle() {
    vulkan.destroy();
    window.destroy();
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
		vulkan.drawFrame();
	}
	vulkan.endDraw();
}
}        // namespace App