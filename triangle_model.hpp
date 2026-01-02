#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "triangle_device.hpp"
#include <cstdint>
#include <vulkan/vulkan_core.h>
#include <glm/ext/vector_float2.hpp>
#include <vector>

namespace triangle {
class TriangleModel{
public:

    struct Vertex {
        glm::vec2 position;

        static std::vector<VkVertexInputBindingDescription>
        getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription>
        getAttributeDescriptions();
    };
    TriangleModel(TriangleDevice &device, std::vector<Vertex> &vertices);
    TriangleModel(const TriangleModel &) = delete;
    TriangleModel &operator=(const TriangleModel &) = delete;
    ~TriangleModel();

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

  private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    TriangleDevice &triangleDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

};
}
