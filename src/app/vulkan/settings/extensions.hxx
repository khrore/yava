#include <vulkan/vulkan.h>

#include <vector>

namespace App {
namespace Settings {
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
}
}