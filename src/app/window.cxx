#include "window.hxx"

#include "app/settings/core.hxx"

namespace App
{
Window::Window()
{
	glfwInit();
	// disable OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// make window disable resize for quality of life
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// create window
	window = glfwCreateWindow(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
}        // namespace App