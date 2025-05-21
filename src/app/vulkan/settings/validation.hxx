#include <vector>

namespace App
{
namespace Settings
{

const std::vector<const char *> enabledValidationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
constexpr bool isEnableValidationLayers = false;
#else
constexpr bool isEnableValidationLayers = false;
#endif

}        // namespace Settings
}        // namespace App