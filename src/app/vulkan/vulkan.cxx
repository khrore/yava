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
	createRenderPass();
	createGraphicsPipline();
	createFramebuffer();
	createCommandPool();
	createCommandBuffer();
    createSyncObjects();
}

Vulkan::~Vulkan()
{
    destroySyncObjects();
	destroyCommandPool();
	destroyFramebuffer();
	destroyGraphicsPipline();
	destroyRenderPass();
	destoryImageViews();
	destroySwapChain();
	destroyDevice();
	destroyDebugMessager();
	destroySurface();
	destroyInstance();
}
}        // namespace App