#include <vulkan/vulkan.h>

namespace App
{
extern void createBuffer(
    VkDevice              device,
    VkPhysicalDevice      physicalDevice,
    VkDeviceSize          size,
    VkBufferUsageFlags    usage,
    VkMemoryPropertyFlags properties,
    VkBuffer             &buffer,
    VkDeviceMemory       &bufferMemory);
}