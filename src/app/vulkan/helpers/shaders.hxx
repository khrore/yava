#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

namespace App
{
extern VkShaderModule createShaderModule(
    const std::vector<char> &code,
    VkDevice                &device);
}        // namespace App