#include "app/vulkan/vulkan.hxx"

#include "app/vulkan/settings/frames.hxx"

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace App
{
void Vulkan::createModelViewProjectionMatrix()
{
	VkDeviceSize mvpBufferSize =
	    sizeof(VkHelpers::ModelViewProjectionMatrix);

	mvpBuffers.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	mvpBuffersMemory.resize(Settings::MAX_FRAMES_IN_FLIGHT);
	mvpBuffersMapped.resize(Settings::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT;
	     i++)
	{
		VkHelpers::createBuffer(
		    vkContext.device, vkContext.physicalDevice,
		    mvpBufferSize,
		    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		    mvpBuffers[i], mvpBuffersMemory[i]);

		vkMapMemory(vkContext.device, mvpBuffersMemory[i],
		            0, mvpBufferSize, 0,
		            &mvpBuffersMapped[i]);
	}
}

void Vulkan::updateModelViewProjectionMatrix(
    uint32_t currentFrame)
{
	static auto startTime =
	    std::chrono::high_resolution_clock::now();
	auto currentTime =
	    std::chrono::high_resolution_clock::now();
	auto time =
	    std::chrono::duration<float,
	                          std::chrono::seconds::period>(
	        currentTime - startTime)
	        .count();

	VkHelpers::ModelViewProjectionMatrix mvp{};
	mvp.model = glm::rotate(glm::mat4(1.0f),
	                        glm::radians(90.0f) * time,
	                        glm::vec3(0.0f, 0.0f, 1.0f));
	mvp.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
	                        glm::vec3(0.0f, 0.0f, 0.0f),
	                        glm::vec3(0.0f, 0.0f, 1.0f));
	mvp.proj  = glm::perspective(
        glm::radians(45.0f),
        swapChain.extent.width /
            (float) swapChain.extent.height,
        0.1f, 10.0f);

	// flip Y, because glm originaly designed for OpenGL
	mvp.proj[1][1] *= -1;

	memcpy(mvpBuffersMapped[currentFrame], &mvp,
	       sizeof(mvp));
}

void Vulkan::destroyModelViewProjectionMatrix()
{
	for (size_t i = 0; i < Settings::MAX_FRAMES_IN_FLIGHT;
	     i++)
	{
		vkDestroyBuffer(vkContext.device, mvpBuffers[i],
		                nullptr);
		vkFreeMemory(vkContext.device, mvpBuffersMemory[i],
		             nullptr);
	}
}
}        // namespace App