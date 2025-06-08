#include "GLFW/glfw3.h"
#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/helpers/buffer.hxx"

#include "app/vulkan/settings/frames.hxx"
#include <stdexcept>

#define GLM_FORCE_RADIANS 1
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

void Vulkan::createDescriptorPool()
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(Settings::MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes    = &poolSize;
	poolInfo.maxSets       = static_cast<uint32_t>(Settings::MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(
	        device,
	        &poolInfo,
	        nullptr,
	        &descriptorPool) !=
	    VK_SUCCESS)
	{
		throw std::runtime_error("failed to create desciptor pool!");
	}
}

void Vulkan::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(
	    Settings::MAX_FRAMES_IN_FLIGHT,
	    descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount =
	    static_cast<uint32_t>(Settings::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(
	        device,
	        &allocInfo,
	        descriptorSets.data()) !=
	    VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate desciptor sets!");
	}

	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range  = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet           = descriptorSets[i];
		descriptorWrite.dstBinding       = 0;
		descriptorWrite.dstArrayElement  = 0;
		descriptorWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount  = 1;
		descriptorWrite.pBufferInfo      = &bufferInfo;
		descriptorWrite.pImageInfo       = nullptr;        // optional
		descriptorWrite.pTexelBufferView = nullptr;        // optional

		vkUpdateDescriptorSets(
		    device,
		    1,
		    &descriptorWrite,
		    0,
		    nullptr);
	}
}

void Vulkan::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(Settings::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT; i++)
	{
		createBuffer(
		    device,
		    physicalDevice,
		    bufferSize,
		    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		    uniformBuffers[i],
		    uniformBuffersMemory[i]);

		vkMapMemory(
		    device,
		    uniformBuffersMemory[i],
		    0,
		    bufferSize,
		    0,
		    &uniformBuffersMapped[i]);
	}
}

void Vulkan::updateUniformBuffer(uint32_t currentFrame)
{
	float time = (float) glfwGetTime() - startTime;
	startTime  = (float) glfwGetTime();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(
	    glm::mat4(1.0f),
	    glm::radians(90.0f) * time,
	    glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(
	    glm::vec3(2.0f, 2.0f, 2.0f),
	    glm::vec3(0.0f, 0.0f, 0.0f),
	    glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(
	    glm::radians(45.0f),
	    swapChainExtent.width / (float) swapChainExtent.height,
	    0.1f,
	    10.0f);

	// flip Y, because glm originaly designed for OpenGL
	ubo.proj[1][1] *= -1;

	memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}

void Vulkan::destroyUniformBuffers()
{
	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroyBuffer(
		    device,
		    uniformBuffers[i],
		    nullptr);
		vkFreeMemory(
		    device,
		    uniformBuffersMemory[i],
		    nullptr);
	}
}

void Vulkan::destroyDescriptorPool()
{
	vkDestroyDescriptorPool(
	    device,
	    descriptorPool,
	    nullptr);
}

void Vulkan::destrpyDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(
	    device,
	    descriptorSetLayout,
	    nullptr);
}
}        // namespace App