#include "helpers.hxx"

#include <stdexcept>

namespace App
{
void VkHelpers::createImage(
    VkDevice device, VkPhysicalDevice physicalDevice,
    uint32_t width, uint32_t height, uint32_t mipLevels,
    VkSampleCountFlagBits numSamples, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties, VkImage &image,
    VkDeviceMemory &imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType     = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width  = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth  = 1;
	imageInfo.mipLevels     = mipLevels;
	imageInfo.arrayLayers   = 1;
	imageInfo.format        = format;
	imageInfo.tiling        = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage         = usage;
	imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples       = numSamples;
	imageInfo.flags         = 0;        // optional

	if (vkCreateImage(device, &imageInfo, nullptr,
	                  &image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image,
	                             &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize  = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(
	    physicalDevice, memRequirements.memoryTypeBits,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(device, &allocInfo, nullptr,
	                     &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to allocate image memory!");
	}

	vkBindImageMemory(device, image, imageMemory, 0);
}

VkImageView VkHelpers::createImageView(
    VkDevice device, VkImage image, uint32_t mipLevels,
    VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType =
	    VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image    = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format   = format;
	viewInfo.subresourceRange.aspectMask     = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel   = 0;
	viewInfo.subresourceRange.levelCount     = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount     = 1;
	// left out the explicit viewInfo.components
	// initialization, because VK_COMPONENT_SWIZZLE_IDENTITY
	// is defined as 0 anyway

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr,
	                      &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create texture image view!");
	}

	return imageView;
}

void VkHelpers::translationImageLayout(
    VkDevice device, VkQueue graphicQueue,
    VkCommandPool commandPool, VkImage image,
    uint32_t mipLevels, VkFormat format,
    VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer =
	    beginSingleTimeCommands(device, commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout           = oldLayout;
	barrier.newLayout           = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image               = image;

	if (newLayout ==
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask =
		    VK_IMAGE_ASPECT_DEPTH_BIT;

		if (hasStancilComponent(format))
		{
			barrier.subresourceRange.aspectMask |=
			    VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask =
		    VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel   = 0;
	barrier.subresourceRange.levelCount     = mipLevels;
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
	else if (
	    oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
	    newLayout ==
	        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask =
		    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
		    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage =
		    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
	{
		throw std::invalid_argument(
		    "unsupported layout translation!");
	}

	vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage,
	                     0, 0, nullptr, 0, nullptr, 1,
	                     &barrier);

	endSingleTimeCommands(device, graphicQueue,
	                      commandBuffer, commandPool);
}
}        // namespace App