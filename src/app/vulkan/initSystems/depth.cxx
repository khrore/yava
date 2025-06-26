#include "app/vulkan/vulkan.hxx"

namespace App
{
void Vulkan::createDepthResources()
{
	VkFormat depthFormat = VkHelpers::findDepthFormat(
	    vkContext.physicalDevice);

	VkHelpers::createImage(
	    vkContext.device, vkContext.physicalDevice,
	    swapChain.extent.width, swapChain.extent.height,
	    depthFormat, VK_IMAGE_TILING_OPTIMAL,
	    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage,
	    depthImageMemory);

	depthImageView = VkHelpers::createImageView(
	    vkContext.device, depthImage, depthFormat,
	    VK_IMAGE_ASPECT_DEPTH_BIT);
}
}        // namespace App