#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/model.hxx"

#include <cmath>
#include <cstring>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace App
{
void Vulkan::createTextureImage()
{
	int      texWidth, texHeight, texChannels;
	stbi_uc *pixels = stbi_load(
	    Settings::TEX_MODEL_PATH.c_str(), &texWidth,
	    &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(
	                std::max(texWidth, texHeight)))) +
	            1;

	if (!pixels)
	{
		throw std::runtime_error(
		    "failed to load texture image");
	}

	VkBuffer       stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkHelpers::createBuffer(
	    vkContext.device, vkContext.physicalDevice,
	    imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	    stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(vkContext.device, stagingBufferMemory, 0,
	            imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(vkContext.device, stagingBufferMemory);
	stbi_image_free(pixels);

	VkHelpers::createImage(
	    vkContext.device, vkContext.physicalDevice,
	    texWidth, texHeight, mipLevels,
	    VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB,
	    VK_IMAGE_TILING_OPTIMAL,
	    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
	        VK_IMAGE_USAGE_TRANSFER_DST_BIT |
	        VK_IMAGE_USAGE_SAMPLED_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage,
	    textureImageMemory);

	VkHelpers::translationImageLayout(
	    vkContext.device, vkContext.graphicQueue,
	    commandPool, textureImage, mipLevels,
	    VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
	    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	VkHelpers::copyBufferToImage(
	    vkContext.device, vkContext.graphicQueue,
	    commandPool, stagingBuffer, textureImage,
	    static_cast<uint32_t>(texWidth),
	    static_cast<uint32_t>(texHeight));

	// VkHelpers::translationImageLayout(
	//     vkContext.device, vkContext.graphicQueue,
	//     commandPool, textureImage, mipLevels,
	//     VK_FORMAT_R8G8B8A8_SRGB,
	//     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
	//     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	VkHelpers::generateMipmaps(
	    vkContext, commandPool, textureImage,
	    VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight,
	    mipLevels);

	vkDestroyBuffer(vkContext.device, stagingBuffer,
	                nullptr);
	vkFreeMemory(vkContext.device, stagingBufferMemory,
	             nullptr);
}

void Vulkan::destroyTextureImage()
{
	vkDestroyImage(vkContext.device, textureImage, nullptr);
	vkFreeMemory(vkContext.device, textureImageMemory,
	             nullptr);
}

void Vulkan::createTextureImageView()
{
	textureImageView = VkHelpers::createImageView(
	    vkContext.device, textureImage, mipLevels,
	    VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Vulkan::destroyTextureImageView()
{
	vkDestroyImageView(vkContext.device, textureImageView,
	                   nullptr);
}

void Vulkan::createTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType =
	    VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU =
	    VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV =
	    VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW =
	    VK_SAMPLER_ADDRESS_MODE_REPEAT;

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(vkContext.physicalDevice,
	                              &properties);
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy =
	    properties.limits.maxSamplerAnisotropy;

	samplerInfo.borderColor =
	    VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable           = VK_FALSE;
	samplerInfo.compareOp  = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod     = 0.0f;        // optional
	samplerInfo.maxLod     = static_cast<float>(mipLevels);
	samplerInfo.mipLodBias = 0.0f;        // optional

	if (vkCreateSampler(vkContext.device, &samplerInfo,
	                    nullptr,
	                    &textureSampler) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create texture sampler");
	}
}

void Vulkan::destroyTextureSampler()
{
	vkDestroySampler(vkContext.device, textureSampler,
	                 nullptr);
}
}        // namespace App