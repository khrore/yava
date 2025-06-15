#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/helpers/buffer.hxx"
#include "app/vulkan/helpers/image.hxx"


#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace App
{
void Vulkan::createTextureImage()
{
	int      texWidth, texHeight, texChannels;
	stbi_uc *pixels =
	    stbi_load("assets/textures/c.jpg", &texWidth,
	              &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels)
	{
		throw std::runtime_error(
		    "failed to load texture image");
	}

	VkBuffer       stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(device, physicalDevice, imageSize,
	             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	             stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize,
	            0, &data);
	vkUnmapMemory(device, stagingBufferMemory);
	stbi_image_free(pixels);

	createImage(device, physicalDevice, textureImageMemory,
	            textureImage, texWidth, texHeight,
	            VK_FORMAT_R8G8B8A8_SRGB,
	            VK_IMAGE_TILING_OPTIMAL,
	            VK_IMAGE_USAGE_TRANSFER_DST_BIT |
	                VK_IMAGE_USAGE_SAMPLED_BIT,
	            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	            textureImage, textureImageMemory);
}
}        // namespace App