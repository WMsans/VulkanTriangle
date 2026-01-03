#include "first_app.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace triangle {

FirstApp::FirstApp() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

FirstApp::~FirstApp() { vkDestroyPipelineLayout(triangleDevice.device(), pipelineLayout, nullptr); }

void FirstApp::run() {
  while (!triangleWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(triangleDevice.device());
}

void FirstApp::loadModels() {
  std::vector<TriangleModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  triangleModel = std::make_unique<TriangleModel>(triangleDevice, vertices);
}

void FirstApp::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(triangleDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void FirstApp::recreateSwapChain() {
  auto extent = triangleWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = triangleWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(triangleDevice.device());
  if (triangleSwapChain == nullptr) {
    triangleSwapChain = std::make_unique<TriangleSwapChain>(triangleDevice, extent);
  } else {
    std::shared_ptr<TriangleSwapChain> oldSwapChain = std::move(triangleSwapChain);
    triangleSwapChain = std::make_unique<TriangleSwapChain>(triangleDevice, extent, oldSwapChain);
    assert(
        triangleSwapChain->imageCount() == oldSwapChain->imageCount() &&
        "Swap chain image count has changed!");
  }
  createPipeline();
}

void FirstApp::createPipeline() {
  PipelineConfigInfo pipelineConfig{};
  TrianglePipeline::defaultPipelineConfigInfo(
      pipelineConfig,
      triangleSwapChain->width(),
      triangleSwapChain->height());
  pipelineConfig.renderPass = triangleSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  trianglePipeline = std::make_unique<TrianglePipeline>(
      triangleDevice,
      "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv",
      pipelineConfig);
}

void FirstApp::createCommandBuffers() {
  commandBuffers.resize(triangleSwapChain->imageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = triangleDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(triangleDevice.device(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void FirstApp::recordCommandBuffer(int imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = triangleSwapChain->getRenderPass();
  renderPassInfo.framebuffer = triangleSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = triangleSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  trianglePipeline->bind(commandBuffers[imageIndex]);
  triangleModel->bind(commandBuffers[imageIndex]);
  triangleModel->draw(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void FirstApp::drawFrame() {
  uint32_t imageIndex;
  auto result = triangleSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result = triangleSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      triangleWindow.wasWindowResized()) {
    triangleWindow.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

}  // namespace triangle
