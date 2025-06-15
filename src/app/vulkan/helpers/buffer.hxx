#include <vulkan/vulkan.h>

namespace App
{
extern void createBuffer(VkDevice           device,
                         VkPhysicalDevice   physicalDevice,
                         VkDeviceSize       size,
                         VkBufferUsageFlags usage,
                         VkMemoryPropertyFlags properties,
                         VkBuffer             &buffer,
                         VkDeviceMemory &bufferMemory);

extern void
    copyBuffer(VkDevice device, VkCommandPool commandPool,
               VkQueue graphicQueue, VkBuffer srcBuffer,
               VkBuffer dstBuffer, VkDeviceSize size);

extern uint32_t
    findMemoryType(VkPhysicalDevice      physicalDevice,
                   uint32_t              typeFilter,
                   VkMemoryPropertyFlags properties);


extern void endSingleTimeCommands(
    VkDevice device, VkCommandBuffer commandBuffer,
    VkCommandPool commandPool, VkQueue graphicQueue);

VkCommandBuffer
    beginSingleTimeCommands(VkDevice      device,
                            VkCommandPool commandPool);

extern void translationImageLayout(
    VkDevice device, VkCommandPool commandPool,
    VkQueue graphicQueue, VkImage image, VkFormat format,
    VkImageLayout oldLayout, VkImageLayout newLayout);
}        // namespace App