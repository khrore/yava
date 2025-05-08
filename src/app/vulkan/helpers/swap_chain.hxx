#pragma once

#define GLFW_INCLUDE_VULKAN 1

#include "GLFW/glfw3.h"

#include <vector>

namespace App
{
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR        capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR>   presentModes;
};

extern SwapChainSupportDetails querySwapChainSupport(
    VkPhysicalDevice physicalDevice,
    VkSurfaceKHR     surface);

extern VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &avaliableFormats);

extern VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &avaliablePresentModes);

VkExtent2D chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities,
    GLFWwindow                     *window);
}        // namespace App