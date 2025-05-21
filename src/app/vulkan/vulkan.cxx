#include "vulkan.hxx"

#include "app/window/window.hxx"

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
	createRenderPass();
	createGraphicsPipline();
	createFramebuffer();
	createCommandPool();
	createCommandBuffer();
    createSyncObjects();
}

Vulkan::~Vulkan()
{
    cleanupSwapChain();

    destroySyncObjects();
	destroyCommandPool();
	destroyGraphicsPipline();
	destroyRenderPass();
	destroyDevice();
	destroyDebugMessager();
	destroySurface();
	destroyInstance();
}
}        // namespace App