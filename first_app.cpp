#include "first_app.hpp"
#include "triangle_model.hpp"
#include "triangle_pipeline.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace triangle {

FirstApp::FirstApp(){
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
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
    auto pipelineConfig = trianglePipeline->defaultPipelineConfigInfo(triangleSwapChain->width(), triangleSwapChain->height());
    pipelineConfig.renderPass = triangleSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    trianglePipeline = std::make_unique<TrianglePipeline>(
        triangleDevice,
        "./shaders/simple_shader.vert.spv",
        "./shaders/simple_shader.frag.spv",
        pipelineConfig
    );
}
void FirstApp::createCommandBuffers() {
    commandBuffers.resize(triangleSwapChain->imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = triangleDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(triangleDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("Failed to create command buffers");
    }

}
void FirstApp::drawFrames() {
    uint32_t imageIndex;
    auto result = triangleSwapChain->acquireNextImage(&imageIndex);

    if(result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("failed to acquire swap chain image");
    }
    
    recordCommandBuffer(imageIndex);
    result = triangleSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || triangleWindow.WasWindowResized()){
        triangleWindow.resetWinodwResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS){
        throw std::runtime_error("failed to acquire swap chain image");
    }
}
void FirstApp::loadModels() {
    std::vector<TriangleModel::Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    triangleModel = std::make_unique<TriangleModel>(triangleDevice, vertices);
}

std::vector<TriangleModel::Vertex> FirstApp::getSierpinskiVertices(glm::vec2 topPos, float sideLen, int num){
    glm::vec2 buttomLeftPos = {topPos.x - sideLen/2, topPos.y + sideLen / 2 * std::sqrt(3.0f)};
    glm::vec2 buttomRightPos = {topPos.x + sideLen/2, topPos.y + sideLen / 2 * std::sqrt(3.0f)};
    if (num <= 0){
        std::vector<TriangleModel::Vertex> result = {
            {{topPos.x, topPos.y}},
            {buttomRightPos},
            {buttomLeftPos},
        };
        return result;
    }
    auto topTriangle = getSierpinskiVertices(topPos, sideLen / 2, num - 1);
    auto buttomLeftTriangle = getSierpinskiVertices(
        (topPos + buttomLeftPos) / 2.0f,
        sideLen / 2, 
        num - 1);
    auto buttomRightTriangle = getSierpinskiVertices((topPos + buttomRightPos) / 2.0f, sideLen / 2, num - 1);

    std::vector<TriangleModel::Vertex> combined;
    combined.insert(combined.end(), topTriangle.begin(), topTriangle.end());
    combined.insert(combined.end(), buttomRightTriangle.begin(), buttomRightTriangle.end());
    combined.insert(combined.end(), buttomLeftTriangle.begin(), buttomLeftTriangle.end());

    return combined;
}
void FirstApp::recreateSwapChain() {
    auto extent = triangleWindow.getExtent();
    while (extent.width == 0 || extent.height == 0){
        extent = triangleWindow.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(triangleDevice.device());
    triangleSwapChain = nullptr;
    triangleSwapChain = std::make_unique<TriangleSwapChain>(triangleDevice, extent);
    createPipeline();
}
void FirstApp::recordCommandBuffer(int i) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("Failed to begin recording command buffer");
        }
        VkRenderPassBeginInfo renderpassInfo{};
        renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpassInfo.renderPass = triangleSwapChain->getRenderPass();
        renderpassInfo.framebuffer = triangleSwapChain->getFrameBuffer(i);
        renderpassInfo.renderArea.offset = {0,0};
        renderpassInfo.renderArea.extent = triangleSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1176f, 0.1176f, 0.1804f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        
        renderpassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderpassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

        trianglePipeline->bind(commandBuffers[i]);
        triangleModel->bind(commandBuffers[i]);
        triangleModel->draw(commandBuffers[i]);

        vkCmdEndRenderPass(commandBuffers[i]);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer");
        }
}
} // namespace triangle
