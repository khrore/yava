#include "app/vulkan/vulkan.hxx"

#include <cstddef>

namespace App
{
void Vulkan::createSwapChain()
{
	VkHelpers::SwapChainSupportDetails swapChainSupport =
	    VkHelpers::querySwapChainSupport(vkContext);

	VkSurfaceFormatKHR surfaceFormat =
	    VkHelpers::chooseSwapSurfaceFormat(
	        swapChainSupport.formats);
	VkPresentModeKHR presentMode =
	    VkHelpers::chooseSwapPresentMode(
	        swapChainSupport.presentModes);
	VkExtent2D extent = VkHelpers::chooseSwapExtent(
	    swapChainSupport.capabilities, &window->get());

	uint32_t imageCount =
	    swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
	    imageCount >
	        swapChainSupport.capabilities.maxImageCount)
	{
		imageCount =
		    swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType =
	    VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface          = vkContext.surface;
	createInfo.minImageCount    = imageCount;
	createInfo.imageFormat      = surfaceFormat.format;
	createInfo.imageColorSpace  = surfaceFormat.colorSpace;
	createInfo.imageExtent      = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage =
	    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VkHelpers::QueueFamilyIndices indices =
	    VkHelpers::findQueueFamilies(vkContext);
	uint32_t queueFamilyIndices[] = {
	    indices.graphicsFamily.value(),
	    indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode =
		    VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode =
		    VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices   = nullptr;
	}

	createInfo.preTransform =
	    swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha =
	    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped     = VK_TRUE;
	if (vkCreateSwapchainKHR(
	        vkContext.device, &createInfo, nullptr,
	        &swapChainContext.instance) != VK_SUCCESS)
	{
		std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(vkContext.device,
	                        swapChainContext.instance,
	                        &imageCount, nullptr);
	swapChainContext.images.resize(imageCount);

	vkGetSwapchainImagesKHR(
	    vkContext.device, swapChainContext.instance,
	    &imageCount, swapChainContext.images.data());

	swapChainContext.imageFormat = surfaceFormat.format;
	swapChainContext.extent      = extent;
}

void Vulkan::cleanupSwapChain()
{
	destroyFramebuffer();
	destoryImageViews();
	destroySwapChain();
}

void Vulkan::recreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(&window->get(), &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(&window->get(), &width,
		                       &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(vkContext.device);

	cleanupSwapChain();

	createSwapChain();
	createImageViews();
	createFramebuffer();
}

void Vulkan::destroySwapChain()
{
	vkDestroySwapchainKHR(vkContext.device,
	                      swapChainContext.instance,
	                      nullptr);
}

void Vulkan::createImageViews()
{
	swapChainContext.imageViews.resize(
	    swapChainContext.images.size());
	for (size_t i = 0; i < swapChainContext.images.size();
	     i++)
	{
		swapChainContext.imageViews[i] =
		    VkHelpers::createImageView(
		        vkContext, swapChainContext.images[i],
		        swapChainContext.imageFormat);
	}
}

void Vulkan::destoryImageViews()
{
	for (auto imageView : swapChainContext.imageViews)
	{
		vkDestroyImageView(vkContext.device, imageView,
		                   nullptr);
	}
}

void Vulkan::createFramebuffer()
{
	swapChainContext.framebuffers.resize(
	    swapChainContext.imageViews.size());

	for (size_t i = 0;
	     i < swapChainContext.imageViews.size(); i++)
	{
		VkImageView attachments[] = {
		    swapChainContext.imageViews[i]};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType =
		    VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass      = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments    = attachments;
		framebufferInfo.width =
		    swapChainContext.extent.width;
		framebufferInfo.height =
		    swapChainContext.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(
		        vkContext.device, &framebufferInfo, nullptr,
		        &swapChainContext.framebuffers[i]) !=
		    VK_SUCCESS)
		{
			throw std::runtime_error(
			    "failed to create framebuffer!");
		}
	}
}

void Vulkan::destroyFramebuffer()
{
	for (auto framebuffer : swapChainContext.framebuffers)
	{
		vkDestroyFramebuffer(vkContext.device, framebuffer,
		                     nullptr);
	}
}
}        // namespace App