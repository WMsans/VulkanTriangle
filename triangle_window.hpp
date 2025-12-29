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

  private:

    void initWindow();
    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
};
}
