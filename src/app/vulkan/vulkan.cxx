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

	createTextureImage();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();

	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffer();
	createSyncObjects();
}

Vulkan::~Vulkan()
{
	cleanupSwapChain();

	destroyDescriptorPool();
	destrpyDescriptorSetLayout();
	destroySyncObjects();
	destroyCommandPool();

	destroyUniformBuffers();
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