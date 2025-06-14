#include "app/vulkan/vulkan.hxx"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace App
{
void Vulkan::createTextureImage()
{
	int      texWidth, texHeight, texChannels;
	stbi_uc *pixels = stbi_load(
	    "assets/textures/c.jpg", &texWidth,
	    &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        throw std::runtime_error("failed to load texture image");
    }
}
}        // namespace App