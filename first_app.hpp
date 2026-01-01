#pragma once

#include "triangle_device.hpp"
#include "triangle_pipeline.hpp"
#include "triangle_swap_chain.hpp"
#include "triangle_window.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace triangle {
class FirstApp{

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;
    ~FirstApp();

    void run();

  private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrames();

    TriangleWindow triangleWindow{WIDTH, HEIGHT, "Hello Vulcan!"};
    TriangleDevice triangleDevice{triangleWindow};
    TriangleSwapChain triangleSwapChain{triangleDevice, triangleWindow.getExtent()};
    std::unique_ptr<TrianglePipeline> trianglePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
};
}
