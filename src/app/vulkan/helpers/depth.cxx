#include "helpers.hxx"
#include <stdexcept>

namespace App
{
VkFormat VkHelpers::findSupportedFormat(
    VkPhysicalDevice             physicalDevice,
    const std::vector<VkFormat> &candidates,
    VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice,
		                                    format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR &&
		    (props.linearTilingFeatures & features) ==
		        features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
		         (props.optimalTilingFeatures & features) ==
		             features)
		{
			return format;
		}
	}

	throw std::runtime_error(
	    "failed to find supported format!");
}

VkFormat VkHelpers::findDepthFormat(
    VkPhysicalDevice physicalDevice)
{
	return findSupportedFormat(
	    physicalDevice,
	    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
	     VK_FORMAT_D24_UNORM_S8_UINT},
	    VK_IMAGE_TILING_OPTIMAL,
	    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool VkHelpers::hasStancilComponent(VkFormat format)
{
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
	       format == VK_FORMAT_D24_UNORM_S8_UINT;
}
}        // namespace App