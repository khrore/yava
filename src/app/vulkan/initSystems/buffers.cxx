#include "app/vulkan/vulkan.hxx"

namespace App
{
void Vulkan::createVertexBuffer()
{
	VkDeviceSize bufferSize =
	    sizeof(vertices[0]) * vertices.size();

	VkBuffer       stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkHelpers::createBuffer(
	    vkContext, bufferSize,
	    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	    stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(vkContext.device, stagingBufferMemory, 0,
	            bufferSize, 0, &data);

	std::memcpy(data, vertices.data(), (size_t) bufferSize);
	vkUnmapMemory(vkContext.device, stagingBufferMemory);

	VkHelpers::createBuffer(
	    vkContext, bufferSize,
	    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer,
	    vertexBufferMemory);

	VkHelpers::copyBuffer(vkContext, commandPool,
	                      stagingBuffer, vertexBuffer,
	                      bufferSize);

	vkDestroyBuffer(vkContext.device, stagingBuffer,
	                nullptr);
	vkFreeMemory(vkContext.device, stagingBufferMemory,
	             nullptr);
}

void Vulkan::destroyVertexBuffer()
{
	vkDestroyBuffer(vkContext.device, vertexBuffer,
	                nullptr);
	vkFreeMemory(vkContext.device, vertexBufferMemory,
	             nullptr);
}

void Vulkan::createIndexBuffer()
{
	VkDeviceSize bufferSize =
	    sizeof(indices[0]) * indices.size();

	VkBuffer       stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkHelpers::createBuffer(
	    vkContext, bufferSize,
	    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
	        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	    stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(vkContext.device, stagingBufferMemory, 0,
	            bufferSize, 0, &data);

	std::memcpy(data, indices.data(), (size_t) bufferSize);
	vkUnmapMemory(vkContext.device, stagingBufferMemory);

	VkHelpers::createBuffer(
	    vkContext, bufferSize,
	    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer,
	    indexBufferMemory);

	VkHelpers::copyBuffer(vkContext, commandPool,
	                      stagingBuffer, indexBuffer,
	                      bufferSize);

	vkDestroyBuffer(vkContext.device, stagingBuffer,
	                nullptr);
	vkFreeMemory(vkContext.device, stagingBufferMemory,
	             nullptr);
}

void Vulkan::destroyIndexBuffer()
{
	vkDestroyBuffer(vkContext.device, indexBuffer, nullptr);
	vkFreeMemory(vkContext.device, indexBufferMemory,
	             nullptr);
}
}        // namespace App