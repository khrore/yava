#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/extensions.hxx"
#include "app/vulkan/settings/validation.hxx"

#include <set>

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
	        vkContext.instance, vkContext.debugMessenger,
	        &createInfo, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to set up debug messenger!");
	}
}


void Vulkan::destroyDebugMessager()
{
	if (Settings::isEnableValidationLayers)
	{
		VkHelpers::destroyDebugUtilsMessengerEXT(
		    vkContext.instance, vkContext.debugMessenger,
		    nullptr);
	}
}

void Vulkan::createLogicalDevice()
{
	VkHelpers::QueueFamilyIndices indices =
	    VkHelpers::findQueueFamilies(
	        vkContext.physicalDevice, vkContext.surface);

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

	if (vkCreateDevice(vkContext.physicalDevice,
	                   &createInfo, nullptr,
	                   &vkContext.device) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create logical device!");
	}

	vkGetDeviceQueue(vkContext.device,
	                 indices.graphicsFamily.value(), 0,
	                 &vkContext.graphicQueue);

	vkGetDeviceQueue(vkContext.device,
	                 indices.presentFamily.value(), 0,
	                 &vkContext.presentQueue);
}

void Vulkan::pickPhysicalDevice()
{
	vkContext.physicalDevice = VK_NULL_HANDLE;

	// enumerate devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vkContext.instance,
	                           &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error(
		    "failed to find GPUs with Vulkan "
		    "support!");
	}

	// store enumerated devices
	std::vector<VkPhysicalDevice> physicalDevices(
	    deviceCount);
	vkEnumeratePhysicalDevices(vkContext.instance,
	                           &deviceCount,
	                           physicalDevices.data());
	for (const auto &physicalDevice : physicalDevices)
	{
		if (VkHelpers::isDeviceSuitable(physicalDevice,
		                                vkContext.surface))
		{
			vkContext.physicalDevice = physicalDevice;
			break;
		}
	}
	if (vkContext.physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error(
		    "failed to find a suitable GPU!");
	}
}

void Vulkan::destroyDevice()
{
	vkDestroyDevice(vkContext.device, nullptr);
}
}        // namespace App