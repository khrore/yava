#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/validation.hxx"
#include "app/vulkan/helpers/debug.hxx"

namespace App
{

void Vulkan::setupDebugMessenger()
{
	if (!Settings::isEnableValidationLayers)
		return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugUtilsMessengerCreateInfoEXT(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr,
	                                 &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance                   instance,
    VkDebugUtilsMessengerEXT     debugMessenger,
    const VkAllocationCallbacks *pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

void Vulkan::destroyDebugMessager()
{
	if (Settings::isEnableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
}
}        // namespace App