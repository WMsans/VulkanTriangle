#pragma once

#include "triangle_window.hpp"

namespace triangle {
class FirstApp{

public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

  private: 
    TriangleWindow triangleWindow{WIDTH, HEIGHT, "Hello Vulcan!"};
};
}
