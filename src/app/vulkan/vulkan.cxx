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
	createCommandPool();
	createDescriptorSetLayout();

	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipline();
	createDepthResources();
	createFramebuffer();

	createTextureImage();
	createTextureImageView();
	createTextureSampler();

	createVertexBuffer();
	createIndexBuffer();
	createModelViewProjectionMatrix();

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

	destroyModelViewProjectionMatrix();
	destroyIndexBuffer();
	destroyVertexBuffer();

	destroyTextureSampler();
	destroyTextureImageView();
	destroyTextureImage();

	destroyGraphicsPipline();
	destroyRenderPass();

	destroyDevice();
	destroyDebugMessager();
	destroySurface();
	destroyInstance();
}
}        // namespace App