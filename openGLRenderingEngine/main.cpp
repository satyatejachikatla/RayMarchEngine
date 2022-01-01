#include <Window/Window.hpp>

#include <iostream>
#include <stdexcept>


using namespace std;

int main() {
    int width = 640;
    int height = 480;
    try
    {
        Window window{width,height};
        window.display();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}