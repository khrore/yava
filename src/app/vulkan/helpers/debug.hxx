#pragma once

#include "app/vulkan/helpers/structs.hxx"
#include <iostream>

namespace App
{
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT massageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT        massageType,
    const VkDebugUtilsMessengerCallbackDataEXT
         *pCallbackData,
    void *pUserData)
{
	std::cerr << "validation layer: "
	          << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

extern void populateDebugUtilsMessengerCreateInfoEXT(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo);

extern VkResult CreateDebugUtilsMessengerEXT(
    AppVkEnviroment                           &env,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator);
}        // namespace App