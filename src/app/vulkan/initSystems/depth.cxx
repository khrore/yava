#include "app/vulkan/vulkan.hxx"

namespace App
{
void Vulkan::createDepthResources()
{
	VkFormat depthFormat = VkHelpers::findDepthFormat(
	    vkContext.physicalDevice);

	VkHelpers::createImage(
	    vkContext.device, vkContext.physicalDevice,
	    swapChain.extent.width, swapChain.extent.height, 1,
	    depthFormat, VK_IMAGE_TILING_OPTIMAL,
	    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage,
	    depthImageMemory);

	depthImageView = VkHelpers::createImageView(
	    vkContext.device, depthImage, 1, depthFormat,
	    VK_IMAGE_ASPECT_DEPTH_BIT);

	VkHelpers::translationImageLayout(
	    vkContext.device, vkContext.graphicQueue,
	    commandPool, depthImage, 1, depthFormat,
	    VK_IMAGE_LAYOUT_UNDEFINED,
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

void Vulkan::destroyDepthResources()
{
	vkDestroyImageView(vkContext.device, depthImageView,
	                   nullptr);
	vkDestroyImage(vkContext.device, depthImage, nullptr);
	vkFreeMemory(vkContext.device, depthImageMemory,
	             nullptr);
}
}        // namespace App