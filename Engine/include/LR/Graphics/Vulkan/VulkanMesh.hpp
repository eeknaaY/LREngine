#ifndef LRVULKANMESH_HPP
#define LRVULKANMESH_HPP

#include "../Interfaces/GenericMesh.hpp"
#include <vulkan/vulkan_raii.hpp>

namespace LR{

// Forward declaration
class VulkanAPI;

class VulkanMesh : public GenericMesh {
public:
    VulkanMesh(VulkanAPI* api, const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies);
    ~VulkanMesh();
	void Bind() override;

    static vk::VertexInputBindingDescription GetBindingDescription();
    static std::array<vk::VertexInputAttributeDescription, 2> GetAttributeDescriptions();

    void CreateBuffers(VulkanAPI* api);
    uint32_t FindMemoryType(uint32_t typeFilter, const vk::PhysicalDeviceMemoryProperties& memProperties, const vk::MemoryPropertyFlags& properties);

    vk::BufferCreateInfo m_VertexBufferInfo;
    vk::BufferCreateInfo m_IndexBufferInfo;

    vk::raii::Buffer m_VertexBuffer{ nullptr };
    vk::raii::DeviceMemory m_VertexMemory{ nullptr };

    vk::raii::Buffer m_IndexBuffer{ nullptr };
    vk::raii::DeviceMemory m_IndexMemory{ nullptr };
};

}

#endif