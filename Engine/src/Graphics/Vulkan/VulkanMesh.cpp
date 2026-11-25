#include "LR/Graphics/Vulkan/VulkanMesh.hpp"
#include "LR/Graphics/Vulkan/VulkanAPI.hpp"

namespace LR {
	VulkanMesh::VulkanMesh(VulkanAPI* api, const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies) {
		// Make a copy of the vectors rather than take ownership
		m_Vertices = Vertices;
		m_Indicies = Indicies;

		CreateBuffers(api);

		Core::Debug::Log("Creating Mesh");
	}

	VulkanMesh::~VulkanMesh() {

	}

	void VulkanMesh::Bind() {
		
	}

	vk::VertexInputBindingDescription VulkanMesh::GetBindingDescription()
	{
		vk::VertexInputBindingDescription description;
		description.binding = 0;
		description.stride = sizeof(Vertex);
		description.inputRate = vk::VertexInputRate::eVertex;

		return description;
	}

	std::array<vk::VertexInputAttributeDescription, 2> VulkanMesh::GetAttributeDescriptions()
	{
		std::array<vk::VertexInputAttributeDescription, 2> description;
		description[0] = { 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Position) };
		description[1] = { 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Color) };

		return description;
	}

	void VulkanMesh::CreateBuffers(VulkanAPI* api) {
		m_VertexBufferInfo.size = sizeof(m_Vertices[0]) * m_Vertices.size();
		m_VertexBufferInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
		m_VertexBufferInfo.sharingMode = vk::SharingMode::eExclusive;

		m_VertexBuffer = vk::raii::Buffer(api->GetDevice(), m_VertexBufferInfo);

		vk::PhysicalDeviceMemoryProperties memProperties = api->GetPhysicalDevice().getMemoryProperties();
		vk::MemoryRequirements memRequirements = m_VertexBuffer.getMemoryRequirements();

		vk::MemoryAllocateInfo memoryAllocateInfo;
		memoryAllocateInfo.allocationSize = memRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, memProperties, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		m_VertexMemory = vk::raii::DeviceMemory(api->GetDevice(), memoryAllocateInfo);

		m_VertexBuffer.bindMemory(m_VertexMemory, 0);
		void* data = m_VertexMemory.mapMemory(0, m_VertexBufferInfo.size);
		memcpy(data, m_Vertices.data(), m_VertexBufferInfo.size);
		m_VertexMemory.unmapMemory();
	}

	uint32_t VulkanMesh::FindMemoryType(uint32_t typeFilter, const vk::PhysicalDeviceMemoryProperties& memProperties, const vk::MemoryPropertyFlags& properties){
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		Core::Debug::LogError("Unable to find suitable memory type.");
	}
		
}

