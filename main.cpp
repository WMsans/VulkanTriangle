#include "first_app.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>

int main(){
    triangle::FirstApp app{};
    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
