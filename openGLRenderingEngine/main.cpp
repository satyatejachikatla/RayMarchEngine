#include <Window/Window.hpp>

#include <iostream>
#include <string>
#include <stdexcept>


using namespace std;

int main() {
    int width = 1080;
    int height = 960;

    string shaderName = "./shader/staged.shader";

    try
    {
        Window window{width,height};
        window.setShaderName(shaderName);
        window.display();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}