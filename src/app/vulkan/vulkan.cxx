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
    createImageViews();
}

Vulkan::~Vulkan()
{
    destoryImageViews();
    destroySwapChain();
    destroyDevice();
	destroyDebugMessager();
    destroySurface();
	destroyInstance();
}
}        // namespace App