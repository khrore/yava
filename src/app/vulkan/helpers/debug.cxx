#include "helpers.hxx"

namespace App
{
void VkHelpers::populateDebugUtilsMessengerCreateInfoEXT(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
	createInfo.sType =
	    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType =
	    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData       = nullptr;
}

VkResult VkHelpers::createDebugUtilsMessengerEXT(
    VkHelpers::VkContext                     &context,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(
	        context.instance,
	        "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(context.instance, pCreateInfo,
		            pAllocator, &context.debugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VkHelpers::destroyDebugUtilsMessengerEXT(
    VkHelpers::VkContext        &context,
    const VkAllocationCallbacks *pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(context.instance,
	                          "vkDestroyDebugUtilsMes"
	                          "sengerEXT");
	if (func != nullptr)
	{
		func(context.instance, context.debugMessenger,
		     pAllocator);
	}
}
}        // namespace App