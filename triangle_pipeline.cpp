#include "triangle_pipeline.hpp"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace triangle{

std::vector<char> TrianglePipeline::readFile(const std::string &filepath) {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};
    if(!file.is_open()){
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}
void TrianglePipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                              const std::string &fragFilepath) {
    auto vertShaderCode = readFile(vertFilepath);
    auto fragShaderCode = readFile(fragFilepath);

    std::cout << "Vertex shader code size: " << vertShaderCode.size() << '\n';
    std::cout << "Fragment shader code size: " << fragShaderCode.size() << '\n';
}
TrianglePipeline::TrianglePipeline(const std::string &vertFilepath,
                                   const std::string &fragFilepath) {
    createGraphicsPipeline(vertFilepath, fragFilepath);
}
} // namespace triangle
