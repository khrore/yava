#pragma once

#include <vulkan/vulkan.h>

namespace App
{
struct AppVkEnviroment
{
	VkInstance               instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice         physicalDevice;
	VkDevice                 device;
	VkQueue                  graphicQueue;
	VkQueue                  presentQueue;
	VkSurfaceKHR             surface;
};
}        // namespace App