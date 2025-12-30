#pragma once

#include "triangle_device.hpp"
#include "triangle_pipeline.hpp"
#include "triangle_window.hpp"

namespace triangle {
class FirstApp{

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

  private: 
    TriangleWindow triangleWindow{WIDTH, HEIGHT, "Hello Vulcan!"};
    TriangleDevice triangleDevice{triangleWindow};
    TrianglePipeline trianglePipeline{triangleDevice, "./shaders/simple_shader.vert.spv", "./shaders/simple_shader.frag.spv", trianglePipeline.defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
}
