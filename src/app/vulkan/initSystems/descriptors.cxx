#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/frames.hxx"


namespace App
{
void Vulkan::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding mvpLayoutBinding{};
	mvpLayoutBinding.binding         = 0;
	mvpLayoutBinding.descriptorCount = 1;
	mvpLayoutBinding.descriptorType =
	    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	mvpLayoutBinding.stageFlags =
	    VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding texSamplerLayoutBinding{};
	texSamplerLayoutBinding.binding         = 1;
	texSamplerLayoutBinding.descriptorCount = 1;
	texSamplerLayoutBinding.descriptorType =
	    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	texSamplerLayoutBinding.pImmutableSamplers = nullptr;
	texSamplerLayoutBinding.stageFlags =
	    VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> binding = {
	    mvpLayoutBinding, texSamplerLayoutBinding};
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType =
	    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount =
	    static_cast<uint32_t>(binding.size());
	layoutInfo.pBindings = binding.data();

	if (vkCreateDescriptorSetLayout(
	        vkContext.device, &layoutInfo, nullptr,
	        &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create descriptor set layout!");
	}
}

void Vulkan::destrpyDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(
	    vkContext.device, descriptorSetLayout, nullptr);
}

void Vulkan::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(
	    Settings::MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type =
	    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(
	    Settings::MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType =
	    VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount =
	    static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets    = static_cast<uint32_t>(
        Settings::MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(vkContext.device, &poolInfo,
	                           nullptr, &descriptorPool) !=
	    VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create desciptor pool!");
	}
}

void Vulkan::destroyDescriptorPool()
{
	vkDestroyDescriptorPool(vkContext.device,
	                        descriptorPool, nullptr);
}

void Vulkan::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(
	    Settings::MAX_FRAMES_IN_FLIGHT,
	    descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool     = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(
	    Settings::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(
	        vkContext.device, &allocInfo,
	        descriptorSets.data()) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to allocate desciptor sets!");
	}

	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT;
	     i++)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = mvpBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range =
		    sizeof(VkHelpers::ModelViewProjectionMatrix);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout =
		    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler   = textureSampler;

		std::array<VkWriteDescriptorSet, 2>
		    descriptorWrites{};
		descriptorWrites[0].sType =
		    VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet     = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType =
		    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo     = &bufferInfo;

		descriptorWrites[1].sType =
		    VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet     = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType =
		    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo      = &imageInfo;

		// descriptorWrites[0].pTexelBufferView =
		//     nullptr;        // optional

		vkUpdateDescriptorSets(
		    vkContext.device,
		    static_cast<uint32_t>(descriptorWrites.size()),
		    descriptorWrites.data(), 0, nullptr);
	}
}
}        // namespace App