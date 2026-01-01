#pragma once

#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace triangle {
class TriangleWindow {
public: 
    TriangleWindow (int w, int h, std::string name);
    ~TriangleWindow ();

    TriangleWindow(const TriangleWindow &) = delete;
    TriangleWindow &operator=(const TriangleWindow &) = delete;

    bool shouldClose();

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

  private:

    void initWindow();
    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
};
}
