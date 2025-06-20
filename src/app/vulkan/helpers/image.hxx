#include <vulkan/vulkan.h>

#include "structs.hxx"

namespace App
{
extern void createImage(AppVkEnviroment &env,
                        uint32_t width, uint32_t height,
                        VkFormat              format,
                        VkImageTiling         tiling,
                        VkImageUsageFlags     usage,
                        VkMemoryPropertyFlags properties,
                        VkImage              &image,
                        VkDeviceMemory       &imageMemory);

extern VkImageView createImageView(VkDevice device,
                                   VkImage  image,
                                   VkFormat format);
}        // namespace App