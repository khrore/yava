#include "vulkan.hxx"

namespace App
{
void Vulkan::drawFrame()
{
	vkWaitForFences(
	    device,
	    1,
	    &inFlightFance,
	    VK_TRUE,
	    UINT64_MAX);

	vkResetFences(
	    device,
	    1,
	    &inFlightFance);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(
	    device,
	    swapChain,
	    UINT64_MAX,
	    imageAvailableSemaphore,
	    VK_NULL_HANDLE,
	    &imageIndex);

	vkResetCommandBuffer(commandBuffer, 0);

	recordCommandBuffer(commandBuffer, imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore          waitSemaphores[] = {imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] =
	    {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores    = waitSemaphores;
	submitInfo.pWaitDstStageMask  = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffer;

	VkSemaphore signalSemaphores[]  = {renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores    = signalSemaphores;

	if (vkQueueSubmit(
	        graphicQueue,
	        1,
	        &submitInfo,
	        inFlightFance) != VK_SUCCESS)
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

	vkQueuePresentKHR(presentQueue, &presentInfo);
}
}        // namespace App