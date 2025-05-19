#include "vulkan.hxx"

#include "settings/frames.hxx"
#include <stdexcept>

namespace App
{
void Vulkan::drawFrame()
{
	uint32_t currentFrame = 0;

	vkWaitForFences(
	    device,
	    1,
	    &inFlightFances[currentFrame],
	    VK_TRUE,
	    UINT64_MAX);

	vkResetFences(
	    device,
	    1,
	    &inFlightFances[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(
	    device,
	    swapChain,
	    UINT64_MAX,
	    imageAvailableSemaphores[currentFrame],
	    VK_NULL_HANDLE,
	    &imageIndex);

	VkResult result = vkAcquireNextImageKHR(
	    device,
	    swapChain,
	    UINT64_MAX,
	    imageAvailableSemaphores[currentFrame],
	    VK_NULL_HANDLE,
	    &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain images!");
	}

	vkResetFences(
	    device,
	    1,
	    &inFlightFances[currentFrame]);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);

	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore          waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] =
	    {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores    = waitSemaphores;
	submitInfo.pWaitDstStageMask  = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[]  = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores    = signalSemaphores;

	if (vkQueueSubmit(
	        graphicQueue,
	        1,
	        &submitInfo,
	        inFlightFances[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores    = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount  = 1;
	presentInfo.pSwapchains     = swapChains;
	presentInfo.pImageIndices   = &imageIndex;

	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % Settings::MAX_FRAMES_IN_FLIGHT;
}

void Vulkan::endDraw()
{
	vkDeviceWaitIdle(device);
}
}        // namespace App