#include "triangle_pipeline.hpp"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace triangle{

std::vector<char> TrianglePipeline::readFile(const std::string &filepath) {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};
    if(!file.is_open()){
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}
void TrianglePipeline::createGraphicsPipeline(
    const std::string &vertFilepath, const std::string &fragFilepath,
    const PipelineConfigInfo configInfo) {
    auto vertShaderCode = readFile(vertFilepath);
    auto fragShaderCode = readFile(fragFilepath);

    std::cout << "Vertex shader code size: " << vertShaderCode.size() << '\n';
    std::cout << "Fragment shader code size: " << fragShaderCode.size() << '\n';
}
TrianglePipeline::TrianglePipeline(TriangleDevice &device,
                                   const std::string &vertFilepath,
                                   const std::string &fragFilepath,
                                   const PipelineConfigInfo configInfo) : triangleDevice(device) {
    createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}
PipelineConfigInfo TrianglePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
    PipelineConfigInfo configInfo{};

    return configInfo;
}
void TrianglePipeline::createShaderModule(const std::vector<char> &code,
                                          VkShaderModule *shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; 
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code. data());

    if (vkCreateShaderModule(triangleDevice.device(), &createInfo, nullptr, shaderModule)){
        throw std::runtime_error("Failed to create shader module");
    }
}
} // namespace triangle
