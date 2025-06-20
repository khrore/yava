#include <vulkan/vulkan.h>

#include "structs.hxx"

namespace App
{
extern void createBuffer(AppVkEnviroment      &env,
                         VkDeviceSize          size,
                         VkBufferUsageFlags    usage,
                         VkMemoryPropertyFlags properties,
                         VkBuffer             &buffer,
                         VkDeviceMemory &bufferMemory);

extern void copyBuffer(AppVkEnviroment &env,
                       VkCommandPool    commandPool,
                       VkQueue          graphicQueue,
                       VkBuffer         srcBuffer,
                       VkBuffer         dstBuffer,
                       VkDeviceSize     size);

extern uint32_t
    findMemoryType(AppVkEnviroment      &env,
                   uint32_t              typeFilter,
                   VkMemoryPropertyFlags properties);


extern void endSingleTimeCommands(
    AppVkEnviroment &env, VkCommandBuffer commandBuffer,
    VkCommandPool commandPool, VkQueue graphicQueue);

VkCommandBuffer
    beginSingleTimeCommands(AppVkEnviroment &env,
                            VkCommandPool    commandPool);

extern void translationImageLayout(
    AppVkEnviroment &env, VkCommandPool commandPool,
    VkQueue graphicQueue, VkImage image, VkFormat format,
    VkImageLayout oldLayout, VkImageLayout newLayout);

extern void copyBufferToImage(AppVkEnviroment &env,
                              VkCommandPool    commandPool,
                              VkQueue          graphicQueue,
                              VkBuffer         buffer,
                              VkImage image, uint32_t width,
                              uint32_t height);
}        // namespace App