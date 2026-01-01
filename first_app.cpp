#include "first_app.hpp"
#include "triangle_pipeline.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
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
        drawFrames();
    }

    vkDeviceWaitIdle(triangleDevice.device());
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
void FirstApp::createCommandBuffers() {
    commandBuffers.resize(triangleSwapChain.imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = triangleDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(triangleDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("Failed to create command buffers");
    }

    for (int i = 0; i < commandBuffers.size(); ++i) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to begin recording command buffer");
        }
        VkRenderPassBeginInfo renderpassInfo{};
        renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpassInfo.renderPass = triangleSwapChain.getRenderPass();
        renderpassInfo.framebuffer = triangleSwapChain.getFrameBuffer(i);
        renderpassInfo.renderArea.offset = {0,0};
        renderpassInfo.renderArea.extent = triangleSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1176f, 0.1176f, 0.1804f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        
        renderpassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderpassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

        trianglePipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer");
        }
    }
}
void FirstApp::drawFrames() {
    uint32_t imageIndex;
    auto result = triangleSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        // TODO:: hangle window resize
        throw std::runtime_error("failed to acquire swap chain image");
    }

    result = triangleSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS){
        throw std::runtime_error("failed to acquire swap chain image");
    }
}
} // namespace triangle
