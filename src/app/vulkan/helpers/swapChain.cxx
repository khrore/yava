#include "swapChain.hxx"

#include <algorithm>
#include <cstdint>
#include <limits>

namespace App
{
SwapChainSupportDetails
    querySwapChainSupport(VkPhysicalDevice physicalDevice,
                          VkSurfaceKHR     surface)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
	    physicalDevice, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(
	    physicalDevice, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
		    physicalDevice, surface, &formatCount,
		    details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
	    physicalDevice, surface, &presentModeCount,
	    nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
		    physicalDevice, surface, &presentModeCount,
		    details.presentModes.data());
	}

	return details;
}

// TODO: learn surface formats
// choose best format instade of first
VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &avaliableFormats)
{
	for (const auto &avaliableFormat : avaliableFormats)
	{
		if (avaliableFormat.format ==
		        VK_FORMAT_B8G8R8_SRGB &&
		    avaliableFormat.colorSpace ==
		        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return avaliableFormat;
		}
	}

	return avaliableFormats[0];
}

// TODO: learn aspects of different present modes
// and choose best one instade of selected
extern VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>
        &avaliablePresentModes)
{
	for (const auto &avaliablePresentMode :
	     avaliablePresentModes)
	{
		if (avaliablePresentMode ==
		    VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return avaliablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

extern VkExtent2D chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities,
    GLFWwindow                     *window)
{
	if (capabilities.currentExtent.width !=
	    std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtend = {
		    static_cast<uint32_t>(width),
		    static_cast<uint32_t>(height)};

		actualExtend.width =
		    std::clamp(actualExtend.width,
		               capabilities.minImageExtent.width,
		               capabilities.maxImageExtent.width);
		actualExtend.height =
		    std::clamp(actualExtend.height,
		               capabilities.minImageExtent.height,
		               capabilities.maxImageExtent.height);

		return actualExtend;
	}
}
}        // namespace App