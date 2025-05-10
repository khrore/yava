#include "vulkan.hxx"

#include <fstream>

namespace App
{
// TODO: add file name to throw error
static std::vector<char> readFile(const std::string &filename)
{
	std::fstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t            fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void Vulkan::createGraphicsPipline()
{
    auto vertShaderCode = readFile("spv/vert.spv");
    auto fragShaderCode = readFile("spv/frag.spv");
}
}        // namespace App