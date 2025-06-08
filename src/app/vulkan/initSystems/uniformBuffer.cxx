#include "app/vulkan/vulkan.hxx"
#include <stdexcept>

namespace App
{
struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

void Vulkan::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType =
	    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;

	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType =
	    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings    = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(
	        device,
	        &layoutInfo,
	        nullptr,
	        &descriptorSetLayout) !=
	    VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void Vulkan::destrpyDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(
        device, 
        descriptorSetLayout, 
        nullptr);
}
}        // namespace App