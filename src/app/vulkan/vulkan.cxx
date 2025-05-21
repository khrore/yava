#include "vulkan.hxx"

#include "app/window/window.hxx"

namespace App
{
void Vulkan::init(Window &window, bool &isFramebufferResized)
{
    this->isFramebufferResized = &isFramebufferResized;
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

void Vulkan::destroy()
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