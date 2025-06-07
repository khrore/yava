#include "app/vulkan/helpers/buffer.hxx"
#include "app/vulkan/vulkan.hxx"
#include <cstddef>
#include <cstring>

namespace App
{

void Vulkan::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer       stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(
	    device,
	    physicalDevice,
	    bufferSize,
	    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	    stagingBuffer,
	    stagingBufferMemory);

	void *data;
	vkMapMemory(
	    device,
	    stagingBufferMemory,
	    0,
	    bufferSize,
	    0,
	    &data);

	std::memcpy(
	    data,
	    vertices.data(),
	    (size_t) bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	createBuffer(
	    device,
	    physicalDevice,
	    bufferSize,
	    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	    vertexBuffer,
	    vertexBufferMemory);

	copyBuffer(
	    device,
	    commandPool,
	    graphicQueue,
	    stagingBuffer,
	    vertexBuffer,
	    bufferSize);

	vkDestroyBuffer(
	    device,
	    stagingBuffer,
	    nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Vulkan::destroyVertexBuffer()
{
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(
	    device,
	    vertexBufferMemory,
	    nullptr);
}
}        // namespace App