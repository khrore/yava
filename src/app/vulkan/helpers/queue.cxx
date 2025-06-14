#include "queue.hxx"

#include <vector>

namespace App
{
extern QueueFamilyIndices
    findQueueFamilies(VkPhysicalDevice physicalDevice,
                      VkSurfaceKHR     surface)
{
	QueueFamilyIndices indices;

	// enumerate queue family
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(
	    physicalDevice, &queueFamilyCount, nullptr);

	// store queue family
	std::vector<VkQueueFamilyProperties> queueFamilis(
	    queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
	    physicalDevice, &queueFamilyCount,
	    queueFamilis.data());

	int32_t queueFamilyIndex = 0;
	for (const auto &queueFamily : queueFamilis)
	{
		VkBool32 isPresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(
		    physicalDevice, queueFamilyIndex, surface,
		    &isPresentSupport);

		if (isPresentSupport)
		{
			indices.presentFamily = queueFamilyIndex;
		}

		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			indices.graphicsFamily = queueFamilyIndex;
		}

		if (indices.isComplite())
		{
			break;
		}

		queueFamilyIndex++;
	}
	return indices;
}
}        // namespace App