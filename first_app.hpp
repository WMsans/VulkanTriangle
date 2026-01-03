#pragma once

#include "triangle_device.hpp"
#include "triangle_model.hpp"
#include "triangle_pipeline.hpp"
#include "triangle_swap_chain.hpp"
#include "triangle_window.hpp"

// std
#include <memory>
#include <vector>

namespace triangle {
class FirstApp {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

 private:
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);

  TriangleWindow triangleWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  TriangleDevice triangleDevice{triangleWindow};
  std::unique_ptr<TriangleSwapChain> triangleSwapChain;
  std::unique_ptr<TriangleModel> triangleModel;
  std::unique_ptr<TrianglePipeline> trianglePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
};
}  // namespace triangle
