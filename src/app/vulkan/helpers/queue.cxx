#include "helpers.hxx"

#include <vector>

namespace App
{
VkHelpers::QueueFamilyIndices VkHelpers::findQueueFamilies(
    VkHelpers::VkContext &context)
{
	QueueFamilyIndices indices;

	// enumerate queue family
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(
	    context.physicalDevice, &queueFamilyCount, nullptr);

	// store queue family
	std::vector<VkQueueFamilyProperties> queueFamilis(
	    queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
	    context.physicalDevice, &queueFamilyCount,
	    queueFamilis.data());

	int32_t queueFamilyIndex = 0;
	for (const auto &queueFamily : queueFamilis)
	{
		VkBool32 isPresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(
		    context.physicalDevice, queueFamilyIndex,
		    context.surface, &isPresentSupport);

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