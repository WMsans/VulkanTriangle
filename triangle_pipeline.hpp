#pragma once

#include "triangle_device.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
namespace triangle {

struct PipelineConfigInfo {

};

class TrianglePipeline {
public:
    TrianglePipeline(const TrianglePipeline &) = delete;
    void operator=(const TrianglePipeline &) = delete;
    TrianglePipeline(TriangleDevice &device, const std::string &vertFilepath,
                     const std::string &fragFilepath,
                     const PipelineConfigInfo configInfo);
    ~TrianglePipeline() {}

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                        uint32_t height);

private:
    static std::vector<char> readFile(const std::string &filepath);

    void createGraphicsPipeline(const std::string &vertFilepath,
                                const std::string &fragFilepath,
                                const PipelineConfigInfo configInfo);

    void createShaderModule(const std::vector<char> &code,
                            VkShaderModule *shaderModule);

    TriangleDevice &triangleDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
}
