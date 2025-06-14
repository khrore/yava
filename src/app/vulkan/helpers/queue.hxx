#pragma once

#include <optional>
#include <vulkan/vulkan.h>

namespace App
{
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplite()
	{
		return graphicsFamily.has_value() &&
		       presentFamily.has_value();
	}
};

extern QueueFamilyIndices
    findQueueFamilies(VkPhysicalDevice physicalDevice,
                      VkSurfaceKHR     surface);
}        // namespace App