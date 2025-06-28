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

	loadModel();

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
	destroySyncObjects();

	destroyModelViewProjectionMatrix();
	destroyIndexBuffer();
	destroyVertexBuffer();

	destroyTextureSampler();
	destroyTextureImageView();
	destroyTextureImage();

	destroyGraphicsPipline();
	destroyRenderPass();

	destrpyDescriptorSetLayout();
	destroyCommandPool();
	destroyDevice();
	// TODO: vkDestroyDebugUtilsMessengerEXT undefined by
	// build destroyDebugMessager();
	destroySurface();
	destroyInstance();
}
}        // namespace App