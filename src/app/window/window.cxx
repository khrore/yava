#include "window.hxx"

#include "GLFW/glfw3.h"
#include "app/settings/core.hxx"

namespace App
{
static void framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
	auto app =
	    reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    app->setFramebufferResized(true);
}

void Window::init(bool &isFramebufferResized)
{
    this->isFramebufferResized = &isFramebufferResized;

	glfwInit();
	// disable OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// make window disable resize for quality of life
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// create window
	window = glfwCreateWindow(
	    Settings::WINDOW_WIDTH,
	    Settings::WINDOW_HEIGHT,
	    "Vulkan",
	    nullptr,
	    nullptr);
    
    // glfwSetWindowUserPointer(window, this);
    // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
}        // namespace App