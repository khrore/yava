#include "buffer.hxx"

#include <stdexcept>

namespace App
{

uint32_t findPhysicalDeviceMemoryType(
    VkPhysicalDevice      physicalDevice,
    uint32_t              typeFilter,
    VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(
	    physicalDevice,
	    &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) &&
		    (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void createBuffer(
    VkDevice              device,
    VkPhysicalDevice      physicalDevice,
    VkDeviceSize          size,
    VkBufferUsageFlags    usage,
    VkMemoryPropertyFlags properties,
    VkBuffer             &buffer,
    VkDeviceMemory       &bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size        = size;
	bufferInfo.usage       = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(
	        device,
	        &bufferInfo,
	        nullptr,
	        &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(
	    device,
	    buffer,
	    &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex =
	    findPhysicalDeviceMemoryType(
	        physicalDevice,
	        memRequirements.memoryTypeBits,
	        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(
	        device,
	        &allocInfo,
	        nullptr,
	        &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(
	    device,
	    buffer,
	    bufferMemory,
	    0);
}

extern void copyBuffer(
    VkDevice      device,
    VkCommandPool commandPool,
    VkQueue       graphicQueue,
    VkBuffer      srcBuffer,
    VkBuffer      dstBuffer,
    VkDeviceSize  size)
{
	// create command buffer
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool        = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(
	    device,
	    &allocInfo,
	    &commandBuffer);

	// record command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(
	    commandBuffer,
	    &beginInfo);

	// transferring contents of buffers
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;        // optional
	copyRegion.dstOffset = 0;        // optional
	copyRegion.size      = size;
	vkCmdCopyBuffer(
	    commandBuffer,
	    srcBuffer,
	    dstBuffer,
	    1,
	    &copyRegion);

	// only containing the copy command
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffer;

	vkQueueSubmit(
	    graphicQueue,
	    1,
	    &submitInfo,
	    VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicQueue);

	vkFreeCommandBuffers(
	    device,
	    commandPool,
	    1,
	    &commandBuffer);
}
}        // namespace App