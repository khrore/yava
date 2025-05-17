#include "vulkan.hxx"

namespace App
{
void Vulkan::createCommandBuffer()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool        = commandPool;
	allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(
	        device,
	        &allocInfo,
	        &commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffer");
	}
}

void Vulkan::destroyCommandBuffer()
{
    
}
}        // namespace App