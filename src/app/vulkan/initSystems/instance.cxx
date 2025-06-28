#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/validation.hxx"


namespace App
{
bool checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount,
	                                   nullptr);

	std::vector<VkLayerProperties> avaliableLayers(
	    layerCount);
	vkEnumerateInstanceLayerProperties(
	    &layerCount, avaliableLayers.data());

	for (const char *layerName :
	     Settings::enabledValidationLayers)
	{
		bool layerFound = false;

		for (const auto &layerProperties : avaliableLayers)
		{
			if (strcmp(layerName,
			           layerProperties.layerName))
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char *> getRequiredGLFWextensions()
{
	uint32_t     glfwExtensionCount = 0;
	const char **glfwExtensions =
	    glfwGetRequiredInstanceExtensions(
	        &glfwExtensionCount);

	std::vector<const char *> extensions(
	    glfwExtensions,
	    glfwExtensions + glfwExtensionCount);

	if (Settings::isEnableValidationLayers)
	{
		extensions.push_back(
		    VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void Vulkan::initInstance()
{
	if (Settings::isEnableValidationLayers &&
	    !checkValidationLayerSupport())
	{
		throw std::runtime_error(
		    "validation layers requested, but "
		    "not avaliable!");
	}

	// creating app info

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName   = "HelloTriangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.pEngineName        = "No Engine";
	appInfo.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion         = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType =
	    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto glfwExtensions = getRequiredGLFWextensions();
	createInfo.enabledExtensionCount =
	    static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames =
	    glfwExtensions.data();

	// debug stuff

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (Settings::isEnableValidationLayers)
	{
		createInfo.enabledLayerCount =
		    static_cast<uint32_t>(
		        Settings::enabledValidationLayers.size());
		createInfo.ppEnabledLayerNames =
		    Settings::enabledValidationLayers.data();
		VkHelpers::populateDebugUtilsMessengerCreateInfoEXT(
		    debugCreateInfo);
		createInfo.pNext =
		    (VkDebugUtilsMessengerCreateInfoEXT
		         *) &debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext             = nullptr;
	}

	// creating instance

	if (vkCreateInstance(&createInfo, nullptr,
	                     &vkContext.instance) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create instance");
	}

	// debug extension

	uint32_t vkExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(
	    nullptr, &vkExtensionCount, nullptr);
	std::vector<VkExtensionProperties> vkExtensions(
	    vkExtensionCount);
	vkEnumerateInstanceExtensionProperties(
	    nullptr, &vkExtensionCount, vkExtensions.data());
}

void Vulkan::destroyInstance()
{
	vkDestroyInstance(vkContext.instance, nullptr);
}
}        // namespace App