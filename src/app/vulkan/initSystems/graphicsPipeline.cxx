#include "app/vulkan/vulkan.hxx"

#include <cstddef>
#include <fstream>
#include <stdexcept>

namespace App
{
VkShaderModule
    createShaderModule(const std::vector<char> &code,
                       VkDevice                &device)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType =
	    VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode =
	    reinterpret_cast<const uint32_t *>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr,
	                         &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create shader module");
	}

	return shaderModule;
}

static std::vector<char>
    readFile(const std::string &filename)
{
	std::ifstream file(filename,
	                   std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t            fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void Vulkan::createGraphicsPipline()
{
	auto vertShaderCode =
	    readFile("assets/shaders/traingle.vert.spv");
	auto fragShaderCode =
	    readFile("assets/shaders/traingle.frag.spv");

	VkShaderModule vertShaderModule = createShaderModule(
	    vertShaderCode, vkContext.device);
	VkShaderModule fragShaderModule = createShaderModule(
	    fragShaderCode, vkContext.device);

	//
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName  = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage =
	    VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName  = "main";

	VkPipelineShaderStageCreateInfo shaderStages[]{
	    vertShaderStageInfo, fragShaderStageInfo};

	std::vector<VkDynamicState> dynamicStates = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_SCISSOR};

	//
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount =
	    static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription =
	    VkHelpers::Vertex::getBindingDescription();
	auto attributeDescriptions =
	    VkHelpers::Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions =
	    &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount =
	    static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions =
	    attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology =
	    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//
	VkViewport viewport{};
	viewport.x        = 0.0f;
	viewport.y        = 0.0f;
	viewport.width    = (float) swapChain.extent.width;
	viewport.height   = (float) swapChain.extent.height;
	viewport.maxDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChain.extent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports    = &viewport;
	viewportState.scissorCount  = 1;
	viewportState.pScissors     = &scissor;

	//
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable        = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth   = 1.0f;
	rasterizer.cullMode    = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	// rasterizer.frontFace               =
	// VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor =
	    0.0f;                                // optional
	rasterizer.depthBiasClamp = 0.0f;        // optional
	rasterizer.depthBiasSlopeFactor =
	    0.0f;        // optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable  = VK_FALSE;
	multisampling.rasterizationSamples = msaaSamples;
	multisampling.sampleShadingEnable  = VK_TRUE;
	multisampling.minSampleShading = .2f;        // optional
	multisampling.pSampleMask = nullptr;         // optional
	multisampling.alphaToCoverageEnable =
	    VK_FALSE;        // optional
	multisampling.alphaToOneEnable =
	    VK_FALSE;        // optional

	//
	VkPipelineColorBlendAttachmentState
	    colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
	    VK_COLOR_COMPONENT_R_BIT |
	    VK_COLOR_COMPONENT_G_BIT |
	    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor =
	    VK_BLEND_FACTOR_ONE;        // optional
	colorBlendAttachment.dstColorBlendFactor =
	    VK_BLEND_FACTOR_ZERO;        // optional
	colorBlendAttachment.colorBlendOp =
	    VK_BLEND_OP_ADD;        // optional
	colorBlendAttachment.srcAlphaBlendFactor =
	    VK_BLEND_FACTOR_ONE;        // optional
	colorBlendAttachment.dstAlphaBlendFactor =
	    VK_BLEND_FACTOR_ZERO;        // optional
	colorBlendAttachment.alphaBlendOp =
	    VK_BLEND_OP_ADD;        // optional

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp =
	    VK_LOGIC_OP_COPY;        // optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments    = &colorBlendAttachment;
	colorBlending.blendConstants[0] =
	    0.0f;        // optional
	colorBlending.blendConstants[1] =
	    0.0f;        // optional
	colorBlending.blendConstants[2] =
	    0.0f;        // optional
	colorBlending.blendConstants[3] =
	    0.0f;        // optional

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable  = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp   = VK_COMPARE_OP_LESS;

	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;        // optional
	depthStencil.maxDepthBounds = 0.0f;        // optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front             = {};        // optional
	depthStencil.back              = {};        // optional

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType =
	    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount =
	    0;        // optional
	pipelineLayoutInfo.pPushConstantRanges =
	    nullptr;        // optional

	if (vkCreatePipelineLayout(
	        vkContext.device, &pipelineLayoutInfo, nullptr,
	        &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType =
	    VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount          = 2;
	pipelineInfo.pStages             = shaderStages;
	pipelineInfo.pVertexInputState   = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState      = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState   = &multisampling;
	pipelineInfo.pDepthStencilState  = &depthStencil;
	pipelineInfo.pColorBlendState    = &colorBlending;
	pipelineInfo.pDynamicState       = &dynamicState;

	pipelineInfo.layout = pipelineLayout;

	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass    = 0;

	pipelineInfo.basePipelineHandle =
	    VK_NULL_HANDLE;                         // optional
	pipelineInfo.basePipelineIndex = -1;        // optional

	if (vkCreateGraphicsPipelines(
	        vkContext.device, VK_NULL_HANDLE, 1,
	        &pipelineInfo, nullptr,
	        &graphicPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error(
		    "failed to create graphics pipeline");
	}

	vkDestroyShaderModule(vkContext.device,
	                      vertShaderModule, nullptr);
	vkDestroyShaderModule(vkContext.device,
	                      fragShaderModule, nullptr);
}

void Vulkan::destroyGraphicsPipline()
{
	vkDestroyPipeline(vkContext.device, graphicPipeline,
	                  nullptr);
	vkDestroyPipelineLayout(vkContext.device,
	                        pipelineLayout, nullptr);
}
}        // namespace App