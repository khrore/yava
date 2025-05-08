#include "vulkan.hxx"

#include "helpers/swap_chain.hxx"

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
	createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
}
}        // namespace App