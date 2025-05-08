#include "vulkan.hxx"

namespace App
{
void Vulkan::createSurface()
{
	if (glfwCreateWindowSurface(
	        instance,
	        &window->get(),
	        nullptr,
	        &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void Vulkan::destroySurface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
}        // namespace App