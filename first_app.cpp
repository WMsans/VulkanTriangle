#include "first_app.hpp"
#include "triangle_pipeline.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace triangle {

FirstApp::FirstApp(){
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

FirstApp::~FirstApp() {
    vkDestroyPipelineLayout(triangleDevice.device(), pipelineLayout, nullptr);

}

void FirstApp::run() {
    while (!triangleWindow.shouldClose()) {
        glfwPollEvents();
    }
}
void FirstApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(triangleDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("Failed to create pipeline layout");
    }
}
void FirstApp::createPipeline() {
    auto pipelineConfig = trianglePipeline->defaultPipelineConfigInfo(triangleSwapChain.width(), triangleSwapChain.height());
    pipelineConfig.renderPass = triangleSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    trianglePipeline = std::make_unique<TrianglePipeline>(
        triangleDevice,
        "./shaders/simple_shader.vert.spv",
        "./shaders/simple_shader.frag.spv",
        pipelineConfig
    );
}
} // namespace triangle
