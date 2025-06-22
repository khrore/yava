#include "helpers.hxx"

namespace App
{
uint32_t VkHelpers::findMemoryType(
    VkHelpers::VkContext &context, uint32_t typeFilter,
    VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(
	    context.physicalDevice, &memProperties);

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
    VkHelpers::VkContext &context, VkDeviceSize size,
    VkBufferUsageFlags    usage,
    VkMemoryPropertyFlags properties, VkBuffer &buffer,
    VkDeviceMemory &bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size  = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(context.device, &bufferInfo, nullptr,
	                   &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(context.device, buffer,
	                              &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize  = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(
	    context, memRequirements.memoryTypeBits,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(context.device, &allocInfo,
	                     nullptr,
	                     &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to allocate buffer memory!");
	}

	vkBindBufferMemory(context.device, buffer, bufferMemory,
	                   0);
}

VkCommandBuffer VkHelpers::beginSingleTimeCommands(
    VkHelpers::VkContext &context,
    VkCommandPool         commandPool)
{
	// create command buffer
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(context.device, &allocInfo,
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
    VkHelpers::VkContext &context,
    VkCommandBuffer       commandBuffer,
    VkCommandPool         commandPool)
{
	// only containing the copy command
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffer;

	vkQueueSubmit(context.graphicQueue, 1, &submitInfo,
	              VK_NULL_HANDLE);
	vkQueueWaitIdle(context.graphicQueue);

	vkFreeCommandBuffers(context.device, commandPool, 1,
	                     &commandBuffer);
}

void VkHelpers::copyBuffer(VkHelpers::VkContext &context,
                           VkCommandPool commandPool,
                           VkBuffer      srcBuffer,
                           VkBuffer      dstBuffer,
                           VkDeviceSize  size)
{
	VkCommandBuffer commandBuffer =
	    beginSingleTimeCommands(context, commandPool);

	// transferring contents of buffers
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;        // optional
	copyRegion.dstOffset = 0;        // optional
	copyRegion.size      = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1,
	                &copyRegion);

	endSingleTimeCommands(context, commandBuffer,
	                      commandPool);
}

void VkHelpers::translationImageLayout(
    VkHelpers::VkContext &context,
    VkCommandPool commandPool, VkImage image,
    VkFormat format, VkImageLayout oldLayout,
    VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer =
	    beginSingleTimeCommands(context, commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout           = oldLayout;
	barrier.newLayout           = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image               = image;

	barrier.subresourceRange.aspectMask =
	    VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel   = 0;
	barrier.subresourceRange.levelCount     = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = 1;

	VkPipelineStageFlags srcStage;
	VkPipelineStageFlags dstStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
	    newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask =
		    VK_ACCESS_TRANSFER_WRITE_BIT;

		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout ==
	             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
	         newLayout ==
	             VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask =
		    VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument(
		    "unsupported layout translation!");
	}

	vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage,
	                     0, 0, nullptr, 0, nullptr, 1,
	                     &barrier);

	endSingleTimeCommands(context, commandBuffer,
	                      commandPool);
}

void VkHelpers::copyBufferToImage(
    VkHelpers::VkContext &context,
    VkCommandPool commandPool, VkBuffer buffer,
    VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer =
	    beginSingleTimeCommands(context, commandPool);

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

	endSingleTimeCommands(context, commandBuffer,
	                      commandPool);
}
}        // namespace App