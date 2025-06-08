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
	createDescriptorSetLayout();
	createGraphicsPipline();
	createFramebuffer();
	createCommandPool();

	createVertexBuffer();
	createIndexBuffer();
	createCommandBuffer();
	createSyncObjects();
}

Vulkan::~Vulkan()
{
	cleanupSwapChain();

    destrpyDescriptorSetLayout();
	destroySyncObjects();
	destroyCommandPool();
	destroyIndexBuffer();
	destroyVertexBuffer();

	destroyGraphicsPipline();
	destroyRenderPass();

	destroyDevice();
	destroyDebugMessager();
	destroySurface();
	destroyInstance();
}
}        // namespace App