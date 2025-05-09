#pragma ones

#define GLFW_INCLUDE_VULKAN 1

#include <GLFW/glfw3.h>
#include <app/window.hxx>


#include <app/pch.hxx>

namespace App
{
class Vulkan
{
  public:
	Vulkan(Window &window);
	~Vulkan();

  private:
	void initInstance();
	void setupDebugMessenger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();

    void destroySwapChain();
	void destroyDevice();
	void destroyDebugMessager();
	void destroySurface();
	void destroyInstance();

  private:
	Window *window;

	VkInstance               instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice         physicalDevice;
	VkDevice                 device;
	VkQueue                  graphicQueue;
	VkQueue                  presentQueue;
	VkSurfaceKHR             surface;
	VkSwapchainKHR           swapChain;
};
}        // namespace App