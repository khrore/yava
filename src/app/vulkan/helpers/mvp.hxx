#include <glm/glm.hpp>

namespace App
{
struct ModelViewProjectionMatrix
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};
}        // namespace App