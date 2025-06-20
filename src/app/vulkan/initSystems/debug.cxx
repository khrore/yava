#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/helpers/debug.hxx"
#include "app/vulkan/settings/validation.hxx"

namespace App
{

void Vulkan::setupDebugMessenger()
{
	if (!Settings::isEnableValidationLayers)
		return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugUtilsMessengerCreateInfoEXT(createInfo);

	if (CreateDebugUtilsMessengerEXT(env, &createInfo,
	                                 nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to set up debug messenger!");
	}
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance                   instance,
    VkDebugUtilsMessengerEXT     debugMessenger,
    const VkAllocationCallbacks *pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(instance,
	                          "vkDestroyDebugUtilsMes"
	                          "sengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

void Vulkan::destroyDebugMessager()
{
	if (Settings::isEnableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(
		    env.instance, env.debugMessenger, nullptr);
	}
}
}        // namespace App