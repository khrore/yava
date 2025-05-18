#pragma ones

#define GLFW_INCLUDE_VULKAN 1

#include <GLFW/glfw3.h>

#include "app/window.hxx"

#include "app/pch.hxx"

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
	void createGraphicsPipline();
	void createFramebuffer();
	void createCommandPool();
	void createCommandBuffer();
	void createSyncObjects();

	void destroySyncObjects();
	void destroyCommandPool();
	void destroyFramebuffer();
	void destroyGraphicsPipline();
	void destroyRenderPass();
	void destoryImageViews();
	void destroySwapChain();
	void destroyDevice();
	void destroyDebugMessager();
	void destroySurface();
	void destroyInstance();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  private:
	Window *window;

	VkInstance                   instance;
	VkDebugUtilsMessengerEXT     debugMessenger;
	VkPhysicalDevice             physicalDevice;
	VkDevice                     device;
	VkQueue                      graphicQueue;
	VkQueue                      presentQueue;
	VkSurfaceKHR                 surface;
	VkSwapchainKHR               swapChain;
	std::vector<VkImage>         swapChainImages;
	VkFormat                     swapChainImageFormat;
	VkExtent2D                   swapChainExtent;
	std::vector<VkImageView>     swapChainImageViews;
	VkRenderPass                 renderPass;
	VkPipelineLayout             pipelineLayout;
	VkPipeline                   graphicPipeline;
	std::vector<VkFramebuffer>   swapChainFramebuffers;
	VkCommandPool                commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore>     imageAvailableSemaphores;
	std::vector<VkSemaphore>     renderFinishedSemaphores;
	std::vector<VkFence>         inFlightFances;
};
}        // namespace App