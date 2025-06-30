#include "app/vulkan/vulkan.hxx"

namespace App
{
void Vulkan::createColorResources()
{
	VkFormat colorFormat = swapChain.imageFormat;

	VkHelpers::createImage(
	    vkContext.device, vkContext.physicalDevice,
	    swapChain.extent.width, swapChain.extent.height, 1,
	    msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL,
	    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
	        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage,
	    colorImageMemory);

	colorImageView = VkHelpers::createImageView(
	    vkContext.device, colorImage, 1, colorFormat,
	    VK_IMAGE_ASPECT_COLOR_BIT);
}

void Vulkan::destroyColorResources()
{
	vkDestroyImageView(vkContext.device, colorImageView,
	                   nullptr);
	vkDestroyImage(vkContext.device, colorImage, nullptr);
	vkFreeMemory(vkContext.device, colorImageMemory,
	             nullptr);
}
}        // namespace App