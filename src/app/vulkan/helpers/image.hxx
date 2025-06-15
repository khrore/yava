#include <vulkan/vulkan.h>

namespace App
{
extern void createImage(
    VkDevice device, VkPhysicalDevice physicalDevice,
    VkDeviceMemory textureImageMemory, VkImage textureImage,
    uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties, VkImage &image,
    VkDeviceMemory &imageMemory);
}