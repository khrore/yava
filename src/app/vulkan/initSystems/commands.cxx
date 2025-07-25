#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/frames.hxx"

namespace App
{
void Vulkan::recordCommandBuffer(
    VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType =
	    VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags            = 0;              // optional
	beginInfo.pInheritanceInfo = nullptr;        // optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) !=
	    VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to begin recording command "
		    "buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType =
	    VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer =
	    swapChain.framebuffers[imageIndex];
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChain.extent;

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount =
	    static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
	                     VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer,
	                  VK_PIPELINE_BIND_POINT_GRAPHICS,
	                  graphicPipeline);

	VkBuffer     vertexBuffers[] = {vertexBuffer};
	VkDeviceSize offsets[]       = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1,
	                       vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0,
	                     VK_INDEX_TYPE_UINT32);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width =
	    static_cast<float>(swapChain.extent.width);
	viewport.height =
	    static_cast<float>(swapChain.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChain.extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBindDescriptorSets(
	    commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
	    pipelineLayout, 0, 1, &descriptorSets[currentFrame],
	    0, nullptr);
	vkCmdDrawIndexed(commandBuffer,
	                 static_cast<uint32_t>(indices.size()),
	                 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create command buffer!");
	}
}

void Vulkan::createCommandBuffer()
{
	commandBuffers.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType =
	    VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = commandBuffers.size();

	if (vkAllocateCommandBuffers(
	        vkContext.device, &allocInfo,
	        commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to allocate command buffer");
	}
}

void Vulkan::createCommandPool()
{
	VkHelpers::QueueFamilyIndices queueFamilyIndices =
	    VkHelpers::findQueueFamilies(
	        vkContext.physicalDevice, vkContext.surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType =
	    VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags =
	    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex =
	    queueFamilyIndices.graphicsFamily.value();
	if (vkCreateCommandPool(vkContext.device, &poolInfo,
	                        nullptr,
	                        &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "faliled to create command pool!");
	}
}

void Vulkan::destroyCommandPool()
{
	vkDestroyCommandPool(vkContext.device, commandPool,
	                     nullptr);
}
}        // namespace App