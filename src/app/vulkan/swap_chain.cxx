#include "vulkan.hxx"

#include "helpers/queue.hxx"
#include "helpers/swap_chain.hxx"
#include <cstddef>
#include <stdexcept>

namespace App
{
void Vulkan::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport =
	    querySwapChainSupport(physicalDevice, surface);

	VkSurfaceFormatKHR surfaceFormat =
	    chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode =
	    chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent =
	    chooseSwapExtent(swapChainSupport.capabilities, &window->get());

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
	    imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface          = surface;
	createInfo.minImageCount    = imageCount;
	createInfo.imageFormat      = surfaceFormat.format;
	createInfo.imageColorSpace  = surfaceFormat.colorSpace;
	createInfo.imageExtent      = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices              = findQueueFamilies(physicalDevice, surface);
	uint32_t           queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices   = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices   = nullptr;
	}

	createInfo.preTransform =
	    swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode    = presentMode;
	createInfo.clipped        = VK_TRUE;
	if (vkCreateSwapchainKHR(
	        device,
	        &createInfo,
	        nullptr,
	        &swapChain) !=
	    VK_SUCCESS)
	{
		std::runtime_error("failed to create swap chain!");
	}
}

void Vulkan::destroySwapChain()
{
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}
}        // namespace App