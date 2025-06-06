#include "app/vulkan/helpers/buffer.hxx"
#include "app/vulkan/vulkan.hxx"
#include <cstddef>
#include <cstring>

namespace App
{

void Vulkan::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	createBuffer(
	    device,
	    physicalDevice,
	    bufferSize,
	    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	    vertexBuffer,
	    vertexBufferMemory);

	void *data;
	vkMapMemory(
	    device,
	    vertexBufferMemory,
	    0,
	    bufferSize,
	    0,
	    &data);

	std::memcpy(
	    data,
	    vertices.data(),
	    bufferSize);
	vkUnmapMemory(device, vertexBufferMemory);
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