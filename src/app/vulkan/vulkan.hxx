#pragma ones

#define GLFW_INCLUDE_VULKAN 1
#include <GLFW/glfw3.h>

#include "app/window/window.hxx"
#include "app/vulkan/helpers/vertex.hxx"
#include "app/vulkan/helpers/structs.hxx"

#include <cstdint>
#include <vector>

struct SwapChain
{
	std::vector<VkImage>       swapChainImages;
	std::vector<VkImageView>   swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkSwapchainKHR             swapChain;
	VkFormat                   swapChainImageFormat;
	VkExtent2D                 swapChainExtent;
};

namespace App
{
class Vulkan
{
  public:
	Vulkan(Window &window);
	~Vulkan();

	void drawFrame();
	void endDraw();

  private:
	void initInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();

	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipline();
	void createFramebuffer();
	void createCommandPool();

	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();

	void createVertexBuffer();
	void createIndexBuffer();
	void createModelViewProjectionMatrix();

	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffer();
	void createSyncObjects();


	void destroyModelViewProjectionMatrix();
	void destroyIndexBuffer();
	void destroyVertexBuffer();

	void destroyTextureSampler();
	void destroyTextureImageView();
	void destroyTextureImage();

	void destroySyncObjects();
	void destroyDescriptorPool();
	void destroyCommandPool();
	void destroyFramebuffer();
	void destroyGraphicsPipline();
	void destrpyDescriptorSetLayout();
	void destroyRenderPass();
	void destoryImageViews();
	void destroySwapChain();

	void destroyDevice();
	void destroyDebugMessager();
	void destroySurface();
	void destroyInstance();


	void recordCommandBuffer(VkCommandBuffer commandBuffer,
	                         uint32_t        imageIndex);
	void updateModelViewProjectionMatrix(
	    uint32_t currentFrame);

	void recreateSwapChain();
	void cleanupSwapChain();

  private:
	Window *window;

	AppVkEnviroment env;

	VkSwapchainKHR             swapChain;
	std::vector<VkImage>       swapChainImages;
	VkFormat                   swapChainImageFormat;
	VkExtent2D                 swapChainExtent;
	std::vector<VkImageView>   swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass                 renderPass;
	VkDescriptorSetLayout        descriptorSetLayout;
	VkDescriptorPool             descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkPipelineLayout             pipelineLayout;
	VkPipeline                   graphicPipeline;
	VkCommandPool                commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence>     inFlightFances;

	uint32_t currentFrame = 0;

	const std::vector<Vertex> vertices = {
	    {{-0.5f, -0.5f, 0.0f},
	     {1.0f, 1.0f, 0.0f},
	     {1.0f, 0.0f}},
	    {{0.5f, -0.5f, 0.0f},
	     {1.0f, 0.0f, 0.0f},
	     {0.0f, 0.0f}},
	    {{0.5f, 0.5f, 0.0f},
	     {0.0f, 1.0f, 0.0f},
	     {0.0f, 1.0f}},
	    {{-0.5f, 0.5f, 0.0f},
	     {0.0f, 0.0f, 1.0f},
	     {1.0f, 1.0f}},

	    {{-0.5f, -0.5f, -0.5f},
	     {1.0f, 1.0f, 0.0f},
	     {1.0f, 0.0f}},
	    {{0.5f, -0.5f, -0.5f},
	     {1.0f, 0.0f, 0.0f},
	     {0.0f, 0.0f}},
	    {{0.5f, 0.5f, -0.5f},
	     {0.0f, 1.0f, 0.0f},
	     {0.0f, 1.0f}},
	    {{-0.5f, 0.5f, -0.5f},
	     {0.0f, 0.0f, 1.0f},
	     {1.0f, 1.0f}}};
	VkBuffer       vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	const std::vector<uint16_t> indices = {
	    0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};
	VkBuffer       indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer>       mvpBuffers;
	std::vector<VkDeviceMemory> mvpBuffersMemory;
	std::vector<void *>         mvpBuffersMapped;

	VkImage        textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView    textureImageView;
	VkSampler      textureSampler;
};
}        // namespace App