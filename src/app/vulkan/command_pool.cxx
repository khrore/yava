#include "vulkan.hxx"

#include "app/vulkan/helpers/queue.hxx"
#include <stdexcept>

namespace App
{
void Vulkan::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices =
	    findQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex =
	    queueFamilyIndices.graphicsFamily.value();
	if (vkCreateCommandPool(
	        device,
	        &poolInfo,
	        nullptr,
	        &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("faliled to create command pool!");
	}
}

void Vulkan::destroyCommandPool()
{
	vkDestroyCommandPool(device, commandPool, nullptr);
}
}        // namespace App