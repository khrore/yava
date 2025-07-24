#include "app/vulkan/vulkan.hxx"
#include <stdexcept>
#include <unordered_map>

#include "app/vulkan/settings/model.hxx"

#define TINYOBJLOADER_IMPLEMENTATION 1
#include <tol/tiny_obj_loader.h>


namespace App
{
void Vulkan::loadModel()
{
	tinyobj::attrib_t                attrib;
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials;
	std::string                      warn, err;

	std::unordered_map<VkHelpers::Vertex, uint32_t>
	    uniqueVertices{};

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials,
	                      &warn, &err,
	                      Settings::OBJ_MODEL_PATH.c_str()))
	{
		throw std::runtime_error(err);
	}

	for (const tinyobj::shape_t &shape : shapes)
	{
		for (const tinyobj::index_t &index :
		     shape.mesh.indices)
		{
			VkHelpers::Vertex vertex{};

			vertex.pos = {
			    attrib.vertices[3 * index.vertex_index + 0],
			    attrib.vertices[3 * index.vertex_index + 1],
			    attrib
			        .vertices[3 * index.vertex_index + 2]};

			vertex.texCoord = {
			    attrib.texcoords[2 * index.texcoord_index +
			                     0],
			    1.0f - attrib.texcoords
			               [2 * index.texcoord_index + 1]};

			vertex.color = {1.0f, 1.0f, 1.0f};

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] =
				    static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
}        // namespace App
