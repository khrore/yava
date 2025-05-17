#include "vulkan.hxx"

namespace App
{
void Vulkan::createSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	if (vkCreateSemaphore(
	        device,
	        &semaphoreInfo,
	        nullptr,
	        &imageAvailableSemaphore) != VK_SUCCESS ||
	    vkCreateSemaphore(
	        device,
	        &semaphoreInfo,
	        nullptr,
	        &renderFinishedSemaphore) != VK_SUCCESS ||
	    vkCreateFence(
	        device,
	        &fenceInfo,
	        nullptr,
	        &inFlightFance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create semaphores and fance!");
	}
}
void Vulkan::destroySyncObjects()
{
	vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
	vkDestroyFence(device, inFlightFance, nullptr);
}
}        // namespace App