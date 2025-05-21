#pragma ones

#define GLFW_INCLUDE_VULKAN 1

#include <GLFW/glfw3.h>

#include "app/window/window.hxx"

#include "app/pch.hxx"

namespace App
{
class Vulkan
{
  public:
    void init(Window &window, bool &isFramebufferResized);
    void destroy();

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

    void recreateSwapChain();
    void cleanupSwapChain();

  private:
	Window *window;
    bool *isFramebufferResized;

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

    uint32_t currentFrame = 0;
};
}        // namespace App