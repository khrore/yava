#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/validation.hxx"

namespace App
{
void Vulkan::createSurface()
{
	if (glfwCreateWindowSurface(
	        vkContext.instance, &window->get(), nullptr,
	        &vkContext.surface) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create window surface!");
	}
}

void Vulkan::destroySurface()
{
	vkDestroySurfaceKHR(vkContext.instance,
	                    vkContext.surface, nullptr);
}
void Vulkan::setupDebugMessenger()
{
	if (!Settings::isEnableValidationLayers)
		return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	VkHelpers::populateDebugUtilsMessengerCreateInfoEXT(
	    createInfo);

	if (VkHelpers::createDebugUtilsMessengerEXT(
	        vkContext, &createInfo, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to set up debug messenger!");
	}
}


void Vulkan::destroyDebugMessager()
{
	if (Settings::isEnableValidationLayers)
	{
		VkHelpers::destroyDebugUtilsMessengerEXT(vkContext,
		                                         nullptr);
	}
}

void Vulkan::createLogicalDevice()
{
	QueueFamilyIndices indices =
	    findQueueFamilies(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreatesInfos;
	std::set<uint32_t> uniqueQueueFamilis = {
	    indices.graphicsFamily.value(),
	    indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilis)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType =
		    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount       = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreatesInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreatesInfos.data();
	createInfo.queueCreateInfoCount =
	    static_cast<uint32_t>(queueCreatesInfos.size());
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount =
	    static_cast<uint32_t>(
	        Settings::deviceExtensions.size());
	createInfo.ppEnabledExtensionNames =
	    Settings::deviceExtensions.data();

	if (Settings::isEnableValidationLayers)
	{
		createInfo.enabledLayerCount =
		    static_cast<uint32_t>(
		        Settings::enabledValidationLayers.size());
		createInfo.ppEnabledLayerNames =
		    Settings::enabledValidationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr,
	                   &device) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(),
	                 0, &graphicQueue);

	vkGetDeviceQueue(device, indices.presentFamily.value(),
	                 0, &presentQueue);
}

void Vulkan::pickPhysicalDevice()
{
	physicalDevice = VK_NULL_HANDLE;

	// enumerate devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount,
	                           nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error(
		    "failed to find GPUs with Vulkan "
		    "support!");
	}

	// store enumerated devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount,
	                           devices.data());
	for (const auto &device : devices)
	{
		if (isDeviceSuitable(device, surface))
		{
			physicalDevice = device;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error(
		    "failed to find a suitable GPU!");
	}
}

void Vulkan::destroyDevice()
{
	vkDestroyDevice(device, nullptr);
}
}        // namespace App