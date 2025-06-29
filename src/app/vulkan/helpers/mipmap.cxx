#include "helpers.hxx"
#include <stdexcept>

// NOTE: this implemintation of mipmaps generation hit
// runtime performance, it could me recreated with
// pregenerated approatch
namespace App
{
void VkHelpers::generateMipmaps(
    VkHelpers::VkContext &context,
    VkCommandPool commandPool, VkImage image,
    VkFormat imageFormat, int32_t texWidth,
    int32_t texHeight, uint32_t mipLevels)
{
	// check if image format supports linerar blitting
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(
	    context.physicalDevice, imageFormat,
	    &formatProperties);

	if (!(formatProperties.optimalTilingFeatures &
	      VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
	{
		throw std::runtime_error(
		    "texture image format does not support linear "
		    "blitting!");
	}

	VkCommandBuffer commandBuffer = beginSingleTimeCommands(
	    context.device, commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask =
	    VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = 1;
	barrier.subresourceRange.levelCount     = 1;

	int32_t mipWidth  = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout =
		    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout =
		    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask =
		    VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(
		    commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		    VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr,
		    0, nullptr, 1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = {0, 0, 0};
		blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
		blit.srcSubresource.aspectMask =
		    VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel       = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount     = 1;
		blit.dstOffsets[0]                 = {0, 0, 0};
		blit.dstOffsets[1]                 = {
            mipWidth > 1 ? mipWidth / 2 : 1,
		    mipHeight > 1 ? mipHeight / 2 : 1, 1};
		blit.dstSubresource.aspectMask =
		    VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel       = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount     = 1;

		vkCmdBlitImage(commandBuffer, image,
		               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		               image,
		               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		               1, &blit, VK_FILTER_LINEAR);

		barrier.oldLayout =
		    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout =
		    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
		    commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
		    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
		    nullptr, 0, nullptr, 1, &barrier);

		if (mipWidth > 1)
			mipWidth /= 2;
		if (mipHeight > 1)
			mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout =
	    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout =
	    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(
	    commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
	    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
	    nullptr, 0, nullptr, 1, &barrier);

	endSingleTimeCommands(context.device,
	                      context.graphicQueue,
	                      commandBuffer, commandPool);
}
}        // namespace App