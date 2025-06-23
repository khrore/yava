#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/frames.hxx"

namespace App
{
void Vulkan::createSyncObjects()
{
	imageAvailableSemaphores.resize(
	    Settings::MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(
	    Settings::MAX_FRAMES_IN_FLIGHT);
	inFlightFances.resize(Settings::MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType =
	    VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT;
	     i++)
	{
		if (vkCreateSemaphore(
		        vkContext.device, &semaphoreInfo, nullptr,
		        &imageAvailableSemaphores[i]) !=
		        VK_SUCCESS ||
		    vkCreateSemaphore(
		        vkContext.device, &semaphoreInfo, nullptr,
		        &renderFinishedSemaphores[i]) !=
		        VK_SUCCESS ||
		    vkCreateFence(vkContext.device, &fenceInfo,
		                  nullptr,
		                  &inFlightFances[i]) != VK_SUCCESS)
		{
			throw std::runtime_error(
			    "failed to create "
			    "synchronization objects for a "
			    "frame!");
		}
	}
}
void Vulkan::destroySyncObjects()
{
	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT;
	     i++)
	{
		vkDestroySemaphore(vkContext.device,
		                   imageAvailableSemaphores[i],
		                   nullptr);
		vkDestroySemaphore(vkContext.device,
		                   renderFinishedSemaphores[i],
		                   nullptr);
		vkDestroyFence(vkContext.device, inFlightFances[i],
		               nullptr);
	}
}
}        // namespace App