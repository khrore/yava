#include "vulkan.hxx"
#include "app/window.hxx"

namespace App
{
Vulkan::Vulkan(Window &window)
{
    this->window = &window;

	initInstance();
	setupDebugMessenger();
    createSurface();
	pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
}

Vulkan::~Vulkan()
{
    destroyDevice();
	destroyDebugMessager();
    destroySurface();
	destroyInstance();
}
}        // namespace App