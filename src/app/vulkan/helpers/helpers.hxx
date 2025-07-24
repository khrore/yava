#include <vulkan/vulkan.h>

#include <functional>
#include <iostream>
#include <optional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#define GLFW_INCLUDE_VULKAN 1
#include <GLFW/glfw3.h>

namespace App
{
class VkHelpers
{
  public:
	// Structs
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
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR>   presentModes;
		VkSurfaceCapabilitiesKHR        capabilities;
	};

	struct VkContext
	{
		VkInstance       instance;
		VkPhysicalDevice physicalDevice;
		VkDevice         device;
		VkQueue          graphicQueue;
		VkQueue          presentQueue;
		VkSurfaceKHR     surface;
	};

	struct SwapChainContext
	{
		std::vector<VkImage>       images;
		std::vector<VkImageView>   imageViews;
		std::vector<VkFramebuffer> framebuffers;
		VkSwapchainKHR             instance;
		VkFormat                   imageFormat;
		VkExtent2D                 extent;
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

		static std::array<VkVertexInputAttributeDescription,
		                  3>
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

		bool operator==(
		    const App::VkHelpers::Vertex &other) const
		{
			return pos == other.pos &&
			       color == other.color &&
			       texCoord == other.texCoord;
		}
	};

	struct Texture
	{
		VkImage        image;
		VkDeviceMemory memory;
		VkImageView    view;
		VkSampler      sampler;
	};
	// Functions
	// Debug


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	    VkDebugUtilsMessageSeverityFlagBitsEXT
	                                    massageSeverity,
	    VkDebugUtilsMessageTypeFlagsEXT massageType,
	    const VkDebugUtilsMessengerCallbackDataEXT
	         *pCallbackData,
	    void *pUserData)
	{
		std::cerr << "validation layer: "
		          << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	static void populateDebugUtilsMessengerCreateInfoEXT(
	    VkDebugUtilsMessengerCreateInfoEXT &createInfo);

	static VkResult createDebugUtilsMessengerEXT(
	    VkInstance               instance,
	    VkDebugUtilsMessengerEXT debugMessenger,
	    const VkDebugUtilsMessengerCreateInfoEXT
	                                *pCreateInfo,
	    const VkAllocationCallbacks *pAllocator);

	static void destroyDebugUtilsMessengerEXT(
	    VkInstance                   instance,
	    VkDebugUtilsMessengerEXT     debugMessenger,
	    const VkAllocationCallbacks *pAllocator);

	// Buffer
	static void createBuffer(
	    VkDevice device, VkPhysicalDevice physicalDevice,
	    VkDeviceSize size, VkBufferUsageFlags usage,
	    VkMemoryPropertyFlags properties, VkBuffer &buffer,
	    VkDeviceMemory &bufferMemory);

	static void copyBuffer(VkDevice      device,
	                       VkQueue       graphicQueue,
	                       VkCommandPool commandPool,
	                       VkBuffer      srcBuffer,
	                       VkBuffer      dstBuffer,
	                       VkDeviceSize  size);

	static uint32_t
	    findMemoryType(VkPhysicalDevice      physicalDevice,
	                   uint32_t              typeFilter,
	                   VkMemoryPropertyFlags properties);


	static void
	    endSingleTimeCommands(VkDevice        device,
	                          VkQueue         graphicQueue,
	                          VkCommandBuffer commandBuffer,
	                          VkCommandPool   commandPool);

	static VkCommandBuffer
	    beginSingleTimeCommands(VkDevice      device,
	                            VkCommandPool commandPool);

	static void translationImageLayout(
	    VkDevice device, VkQueue graphicQueue,
	    VkCommandPool commandPool, VkImage image,
	    uint32_t mipLevels, VkFormat format,
	    VkImageLayout oldLayout, VkImageLayout newLayout);

	static void copyBufferToImage(
	    VkDevice device, VkQueue graphicQueue,
	    VkCommandPool commandPool, VkBuffer buffer,
	    VkImage image, uint32_t width, uint32_t height);

	// Image
	static void createImage(
	    VkDevice device, VkPhysicalDevice physicalDevice,
	    uint32_t width, uint32_t height, uint32_t mipLevels,
	    VkSampleCountFlagBits numSamples, VkFormat format,
	    VkImageTiling tiling, VkImageUsageFlags usage,
	    VkMemoryPropertyFlags properties, VkImage &image,
	    VkDeviceMemory &imageMemory);

	static VkImageView
	    createImageView(VkDevice device, VkImage image,
	                    uint32_t mipLevels, VkFormat format,
	                    VkImageAspectFlags aspectFlags);

	// Swap Chain
	static bool checkDeviceExtensionSupport(
	    VkPhysicalDevice physicalDevice);

	static bool
	    isDeviceSuitable(VkPhysicalDevice physicalDevice,
	                     VkSurfaceKHR     surface);

	static QueueFamilyIndices
	    findQueueFamilies(VkPhysicalDevice physicalDevice,
	                      VkSurfaceKHR     surface);

	static SwapChainSupportDetails querySwapChainSupport(
	    VkPhysicalDevice physicalDevice,
	    VkSurfaceKHR     surface);

	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
	    const std::vector<VkSurfaceFormatKHR>
	        &avaliableFormats);

	static VkPresentModeKHR chooseSwapPresentMode(
	    const std::vector<VkPresentModeKHR>
	        &avaliablePresentModes);

	static VkExtent2D chooseSwapExtent(
	    const VkSurfaceCapabilitiesKHR &capabilities,
	    GLFWwindow                     *window);

	static VkFormat findSupportedFormat(
	    VkPhysicalDevice             physicalDevice,
	    const std::vector<VkFormat> &candidates,
	    VkImageTiling                tiling,
	    VkFormatFeatureFlags         features);

	static VkFormat
	    findDepthFormat(VkPhysicalDevice physicalDevice);

	static bool hasStancilComponent(VkFormat format);

	static void
	    generateMipmaps(VkHelpers::VkContext &context,
	                    VkCommandPool         commandPool,
	                    VkImage image, VkFormat imageFormat,
	                    int32_t texWidth, int32_t texHeight,
	                    uint32_t mipLevels);

	// MSAA
	static VkSampleCountFlagBits getMaxUsableSampleCount(
	    VkPhysicalDevice physicalDevice);
};
}        // namespace App

namespace std
{
template <>
struct hash<App::VkHelpers::Vertex>
{
	size_t operator()(
	    App::VkHelpers::Vertex const &vertex) const
	{
		return (hash<glm::vec3>()(vertex.pos) ^
		        (hash<glm::vec3>()(vertex.color) << 1)) ^
		       (hash<glm::vec2>()(vertex.texCoord) << 1);
	}
};
}        // namespace std
