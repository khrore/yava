#include "helpers.hxx"

namespace App
{
uint32_t VkHelpers::findMemoryType(
    VkPhysicalDevice physicalDevice, uint32_t typeFilter,
    VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice,
	                                    &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount;
	     i++)
	{
		if ((typeFilter & (1 << i)) &&
		    (memProperties.memoryTypes[i].propertyFlags &
		     properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error(
	    "failed to find suitable memory type!");
}

void VkHelpers::createBuffer(
    VkDevice device, VkPhysicalDevice physicalDevice,
    VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkBuffer &buffer,
    VkDeviceMemory &bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size  = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr,
	                   &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer,
	                              &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize  = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(
	    physicalDevice, memRequirements.memoryTypeBits,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(device, &allocInfo, nullptr,
	                     &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

VkCommandBuffer VkHelpers::beginSingleTimeCommands(
    VkDevice device, VkCommandPool commandPool)
{
	// create command buffer
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo,
	                         &commandBuffer);

	// record command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType =
	    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags =
	    VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void VkHelpers::endSingleTimeCommands(
    VkDevice device, VkQueue graphicQueue,
    VkCommandBuffer commandBuffer,
    VkCommandPool   commandPool)
{
	// only containing the copy command
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffer;

	vkQueueSubmit(graphicQueue, 1, &submitInfo,
	              VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicQueue);

	vkFreeCommandBuffers(device, commandPool, 1,
	                     &commandBuffer);
}

void VkHelpers::copyBuffer(VkDevice      device,
                           VkQueue       graphicQueue,
                           VkCommandPool commandPool,
                           VkBuffer      srcBuffer,
                           VkBuffer      dstBuffer,
                           VkDeviceSize  size)
{
	VkCommandBuffer commandBuffer =
	    beginSingleTimeCommands(device, commandPool);

	// transferring contents of buffers
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;        // optional
	copyRegion.dstOffset = 0;        // optional
	copyRegion.size      = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1,
	                &copyRegion);

	endSingleTimeCommands(device, graphicQueue,
	                      commandBuffer, commandPool);
}

void VkHelpers::copyBufferToImage(
    VkDevice device, VkQueue graphicQueue,
    VkCommandPool commandPool, VkBuffer buffer,
    VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer =
	    beginSingleTimeCommands(device, commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset      = 0;
	region.bufferRowLength   = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask =
	    VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel       = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount     = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {width, height, 1};

	vkCmdCopyBufferToImage(
	    commandBuffer, buffer, image,
	    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endSingleTimeCommands(device, graphicQueue,
	                      commandBuffer, commandPool);
}
}        // namespace App