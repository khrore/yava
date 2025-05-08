#pragma once

#include <iostream>
#include <vulkan/vulkan.h>

namespace App
{
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      massageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             massageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void                                       *pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

extern void populateDebugUtilsMessengerCreateInfoEXT(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo);

extern VkResult CreateDebugUtilsMessengerEXT(
    VkInstance                                instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator,
    VkDebugUtilsMessengerEXT                 *pDebugMessenger);
}        // namespace App