#include "triangle_window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace triangle{

TriangleWindow::TriangleWindow(int w, int h, std::string name) : width(w), height(h), windowName(name){
    initWindow();
}

TriangleWindow::~TriangleWindow(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void TriangleWindow::initWindow (){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
bool TriangleWindow::shouldClose() {
    return glfwWindowShouldClose(window);
}

void TriangleWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
        throw std::runtime_error("Failed to create window: " + windowName);
    }
}
} // namespace triangle
