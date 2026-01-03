#include "triangle_window.hpp"

// std
#include <stdexcept>

namespace triangle {

TriangleWindow::TriangleWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
  initWindow();
}

TriangleWindow::~TriangleWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void TriangleWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void TriangleWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to craete window surface");
  }
}

void TriangleWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto triangleWindow = reinterpret_cast<TriangleWindow *>(glfwGetWindowUserPointer(window));
  triangleWindow->framebufferResized = true;
  triangleWindow->width = width;
  triangleWindow->height = height;
}

}  // namespace triangle
