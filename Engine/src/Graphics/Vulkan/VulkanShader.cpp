#include <LR/Graphics/Vulkan/VulkanShader.hpp>

#include <iostream>
#include "LR/Graphics/Vulkan/VulkanMesh.hpp"

namespace LR {
	VulkanShader::VulkanShader(const std::vector<char>& buffer, const vk::raii::Device& device) {
		vk::ShaderModuleCreateInfo createInfo{};
		createInfo.codeSize = buffer.size() * sizeof(char);
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		m_ShaderModule = vk::raii::ShaderModule{ device, createInfo };
	}

	VulkanShader::~VulkanShader() {

	}

	void VulkanShader::Bind() {

	}

	void VulkanShader::CreatePipeline(const vk::Extent2D surfaceExtent, const vk::raii::Device& device, const vk::SurfaceFormatKHR& swapChainSurfaceFormat) {
		try {
			if (m_ShaderModule == nullptr) {
				throw std::runtime_error("Attempted to create pipeline on a null shadermodule.");
			}

			vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
			vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
			vertShaderStageInfo.module = m_ShaderModule;
			vertShaderStageInfo.pName = "vertMain";

			vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
			fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
			fragShaderStageInfo.module = m_ShaderModule;
			fragShaderStageInfo.pName = "fragMain";

			vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

			vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
			inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

			vk::Viewport viewport{ 0.0f, 0.0f, static_cast<float>(surfaceExtent.width), static_cast<float>(surfaceExtent.height), 0.0f, 1.0f };
			vk::Rect2D scissor{ vk::Offset2D{ 0, 0 }, surfaceExtent };

			vk::PipelineViewportStateCreateInfo viewportState;
			viewportState.viewportCount = 1; 
			viewportState.pViewports = &viewport; 
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			vk::PipelineRasterizationStateCreateInfo rasterizer;
			rasterizer.depthClampEnable = vk::False;
			rasterizer.rasterizerDiscardEnable = vk::False;
			rasterizer.polygonMode = vk::PolygonMode::eFill;
			rasterizer.cullMode = vk::CullModeFlagBits::eBack;
			//rasterizer.cullMode = vk::CullModeFlagBits::eNone;
			rasterizer.frontFace = vk::FrontFace::eClockwise;
			rasterizer.frontFace = vk::FrontFace::eClockwise;
			rasterizer.depthBiasEnable = vk::False;
			rasterizer.lineWidth = 1.0f;

			vk::PipelineMultisampleStateCreateInfo multisampling;
			multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
			multisampling.sampleShadingEnable = vk::False;

			vk::PipelineColorBlendAttachmentState colorBlendAttachment;
			colorBlendAttachment.blendEnable = vk::False;
			colorBlendAttachment.colorWriteMask =
				vk::ColorComponentFlagBits::eR |
				vk::ColorComponentFlagBits::eG |
				vk::ColorComponentFlagBits::eB |
				vk::ColorComponentFlagBits::eA;

			vk::PipelineColorBlendStateCreateInfo colorBlending;
			colorBlending.logicOpEnable = vk::False;
			colorBlending.logicOp = vk::LogicOp::eCopy;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;

			vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pushConstantRangeCount = 0;

			vk::raii::PipelineLayout pipelineLayout = vk::raii::PipelineLayout(device, pipelineLayoutInfo);

			vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo;
			pipelineRenderingCreateInfo.colorAttachmentCount = 1;
			pipelineRenderingCreateInfo.pColorAttachmentFormats = &swapChainSurfaceFormat.format;

			std::vector<vk::DynamicState> dynamicStates;
			dynamicStates.push_back(vk::DynamicState::eViewport);
			dynamicStates.push_back(vk::DynamicState::eScissor);

			vk::PipelineDynamicStateCreateInfo dynamicState;
			dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			dynamicState.pDynamicStates = dynamicStates.data();

			auto bindingDescription = VulkanMesh::GetBindingDescription();
			auto attributeDescriptions = VulkanMesh::GetAttributeDescriptions();

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

			vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
			pipelineCreateInfo.stageCount = 2;
			pipelineCreateInfo.pStages = shaderStages;
			pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
			pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
			pipelineCreateInfo.pViewportState = &viewportState;
			pipelineCreateInfo.pRasterizationState = &rasterizer;
			pipelineCreateInfo.pMultisampleState = &multisampling;
			pipelineCreateInfo.pColorBlendState = &colorBlending;
			pipelineCreateInfo.pDynamicState = &dynamicState;
			pipelineCreateInfo.layout = pipelineLayout;
			pipelineCreateInfo.renderPass = nullptr;
			pipelineCreateInfo.pNext = &pipelineRenderingCreateInfo;

			m_Pipeline = vk::raii::Pipeline(
				device,
				nullptr,
				pipelineCreateInfo
			);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
}

