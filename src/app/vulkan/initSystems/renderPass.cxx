#include "app/vulkan/vulkan.hxx"
#include <stdexcept>

namespace App
{
void Vulkan::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format  = swapChain.imageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp =
	    VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp =
	    VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout =
	    VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout =
	    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout =
	    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = VkHelpers::findDepthFormat(
	    vkContext.physicalDevice);
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp =
	    VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp =
	    VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp =
	    VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout =
	    VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout =
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout =
	    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint =
	    VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount    = 1;
	subpass.pColorAttachments       = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask =
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
	    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask =
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
	    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask =
	    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
	    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachment = {
	    colorAttachment, depthAttachment};
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType =
	    VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount =
	    static_cast<uint32_t>(attachment.size());
	renderPassInfo.pAttachments    = attachment.data();
	renderPassInfo.subpassCount    = 1;
	renderPassInfo.pSubpasses      = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies   = &dependency;

	if (vkCreateRenderPass(vkContext.device,
	                       &renderPassInfo, nullptr,
	                       &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "faled to create render pass!");
	}
}

void Vulkan::destroyRenderPass()
{
	vkDestroyRenderPass(vkContext.device, renderPass,
	                    nullptr);
}
}        // namespace App