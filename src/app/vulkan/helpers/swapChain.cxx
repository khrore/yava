#include "helpers.hxx"

#include <algorithm>
#include <cstdint>
#include <limits>

namespace App
{
bool VkHelpers::checkDeviceExtensionSupport(
    VkContext &context)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(
	    physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> avaliableExtensions(
	    extensionCount);
	vkEnumerateDeviceExtensionProperties(
	    physicalDevice, nullptr, &extensionCount,
	    avaliableExtensions.data());

	std::set<std::string> requiredExtensions(
	    Settings::deviceExtensions.begin(),
	    Settings::deviceExtensions.end());

	for (const auto &extension : avaliableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool isDeviceSuitable(VkPhysicalDevice physicalDevice,
                      VkSurfaceKHR     surface)
{
	QueueFamilyIndices indices =
	    findQueueFamilies(physicalDevice, surface);

	bool isExtensionSupported =
	    checkDeviceExtensionSupport(physicalDevice);
	bool isSwapChainAdequate = false;
	if (isExtensionSupported)
	{
		SwapChainSupportDetails swapChainSupport =
		    querySwapChainSupport(physicalDevice, surface);
		isSwapChainAdequate =
		    !swapChainSupport.formats.empty() &&
		    !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice,
	                            &supportedFeatures);

	return indices.isComplite() && isExtensionSupported &&
	       isSwapChainAdequate &&
	       supportedFeatures.samplerAnisotropy;
}
VkHelpers::SwapChainSupportDetails
    VkHelpers::querySwapChainSupport(
        VkHelpers::VkContext &context)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
	    context.physicalDevice, context.surface,
	    &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(
	    context.physicalDevice, context.surface,
	    &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(
		    context.physicalDevice, context.surface,
		    &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
	    context.physicalDevice, context.surface,
	    &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(
		    context.physicalDevice, context.surface,
		    &presentModeCount, details.presentModes.data());
	}

	return details;
}

// TODO: learn surface formats
// choose best format instade of first
VkSurfaceFormatKHR VkHelpers::chooseSwapSurfaceFormat(
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
VkPresentModeKHR VkHelpers::chooseSwapPresentMode(
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

VkExtent2D VkHelpers::chooseSwapExtent(
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