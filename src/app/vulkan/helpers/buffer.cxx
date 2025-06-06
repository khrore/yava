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
}        // namespace App