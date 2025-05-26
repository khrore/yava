#include "vulkan.hxx"

namespace App
{
void Vulkan::createVertexBuffer()
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size        = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(
	        device,
	        &bufferInfo,
	        nullptr,
	        &vertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}
}

void Vulkan::destroyVertexBuffer()
{
	vkDestroyBuffer(device, vertexBuffer, nullptr);
}
}        // namespace App