#include <vector>

namespace App
{
namespace Settings
{

const std::vector<const char *> enabledValidationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool isEnableValidationLayers = true;
#endif

}        // namespace Settings
}        // namespace App