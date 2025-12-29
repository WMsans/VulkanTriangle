#include "first_app.hpp"
#include <GLFW/glfw3.h>

namespace triangle {

void FirstApp::run() {
    while (!triangleWindow.shouldClose()) {
        glfwPollEvents();
    }
}
} // namespace triangle
