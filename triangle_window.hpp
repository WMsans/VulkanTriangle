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

    bool WasWindowResized(){
        return framebufferResized;
    }

    void resetWinodwResizedFlag(){
        framebufferResized = false;
    }

  private:
    static void framebufferResizedCallback(GLFWwindow *window, int width,
                                           int height);

    void initWindow();
    int width;
    int height;
    bool framebufferResized;

    std::string windowName;
    GLFWwindow *window;
};
}
