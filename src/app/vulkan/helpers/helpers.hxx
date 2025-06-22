#include <vulkan/vulkan.h>

#include <iostream>

#include <glm/glm.hpp>

#define GLFW_INCLUDE_VULKAN 1
#include <GLFW/glfw3.h>

namespace App
{
// Structs
struct AppVkContext
{
	VkInstance               instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice         physicalDevice;
	VkDevice                 device;
	VkQueue                  graphicQueue;
	VkQueue                  presentQueue;
	VkSurfaceKHR             surface;
};

struct ModelViewProjectionMatrix
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplite()
	{
		return graphicsFamily.has_value() &&
		       presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR        capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR>   presentModes;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription
	    getBindingDescription()
	{
		VkVertexInputBindingDescription
		    bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride  = sizeof(Vertex);
		bindingDescription.inputRate =
		    VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3>
	    getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3>
		    attributeDescriptions{};

		attributeDescriptions[0].binding  = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format =
		    VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset =
		    offsetof(Vertex, pos);

		attributeDescriptions[1].binding  = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format =
		    VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset =
		    offsetof(Vertex, color);

		attributeDescriptions[2].binding  = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format =
		    VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset =
		    offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};
// Functions
// Debug
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT massageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT        massageType,
    const VkDebugUtilsMessengerCallbackDataEXT
         *pCallbackData,
    void *pUserData)
{
	std::cerr << "validation layer: "
	          << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

void populateDebugUtilsMessengerCreateInfoEXT(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo);

VkResult CreateDebugUtilsMessengerEXT(
    AppVkContext                             &context,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator);

// Buffer
void createBuffer(AppVkContext &context, VkDeviceSize size,
                  VkBufferUsageFlags    usage,
                  VkMemoryPropertyFlags properties,
                  VkBuffer             &buffer,
                  VkDeviceMemory       &bufferMemory);

void copyBuffer(AppVkContext &context,
                VkCommandPool commandPool,
                VkQueue graphicQueue, VkBuffer srcBuffer,
                VkBuffer dstBuffer, VkDeviceSize size);

uint32_t findMemoryType(AppVkContext         &context,
                        uint32_t              typeFilter,
                        VkMemoryPropertyFlags properties);


void endSingleTimeCommands(AppVkContext   &context,
                           VkCommandBuffer commandBuffer,
                           VkCommandPool   commandPool,
                           VkQueue         graphicQueue);

VkCommandBuffer
    beginSingleTimeCommands(AppVkContext &context,
                            VkCommandPool commandPool);

void translationImageLayout(AppVkContext &context,
                            VkCommandPool commandPool,
                            VkQueue       graphicQueue,
                            VkImage image, VkFormat format,
                            VkImageLayout oldLayout,
                            VkImageLayout newLayout);

void copyBufferToImage(AppVkContext &context,
                       VkCommandPool commandPool,
                       VkQueue       graphicQueue,
                       VkBuffer buffer, VkImage image,
                       uint32_t width, uint32_t height);
// Image
void createImage(AppVkContext &context, uint32_t width,
                 uint32_t height, VkFormat format,
                 VkImageTiling         tiling,
                 VkImageUsageFlags     usage,
                 VkMemoryPropertyFlags properties,
                 VkImage              &image,
                 VkDeviceMemory       &imageMemory);

VkImageView createImageView(AppVkContext &context,
                            VkImage image, VkFormat format);

QueueFamilyIndices
    findQueueFamilies(VkPhysicalDevice physicalDevice,
                      VkSurfaceKHR     surface);

SwapChainSupportDetails
    querySwapChainSupport(VkPhysicalDevice physicalDevice,
                          VkSurfaceKHR     surface);

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>
        &avaliableFormats);

VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>
        &avaliablePresentModes);

VkExtent2D chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities,
    GLFWwindow                     *window);
}        // namespace App