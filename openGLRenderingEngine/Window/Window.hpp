#pragma once

#include <GL/glew.h>	// GL Wrangler
#include <GLFW/glfw3.h> // Window management

#include <ScreenQuad/ScreenQuad.hpp>

class Window {

    private:

        GLFWwindow* window;
        int width;
        int height;

        void config();

        ScreenQuad* screenQuad;

    public:
        Window() = delete;
        Window(int width,int height);
        ~Window();

        void display();

        int getWidth() { return width;}
        int getHeight() { return height;}
};