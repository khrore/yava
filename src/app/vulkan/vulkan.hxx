#pragma ones

#define GLFW_INCLUDE_VULKAN 1
#include <GLFW/glfw3.h>

#include "app/window/window.hxx"
#include "helpers/vertex.hxx"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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

	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();

	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffer();
	void createSyncObjects();

	void destroyUniformBuffers();
	void destroyIndexBuffer();
	void destroyVertexBuffer();

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

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void updateUniformBuffer(uint32_t currentFrame);

	void recreateSwapChain();
	void cleanupSwapChain();

  private:
	Window *window;

	VkInstance               instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice         physicalDevice;
	VkDevice                 device;
	VkQueue                  graphicQueue;
	VkQueue                  presentQueue;
	VkSurfaceKHR             surface;

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
	    {{-0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
	    {{0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
	VkBuffer       vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	const std::vector<uint16_t> indices = {
	    0, 1, 2, 2, 3, 0};
	VkBuffer       indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer>       uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void *>         uniformBuffersMapped;

	float startTime = 0;
    float rotate = 0;
};
}        // namespace App