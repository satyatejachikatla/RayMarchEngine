#pragma once

#include <GL/glew.h>	// GL Wrangler
#include <GLFW/glfw3.h> // Window management

#include <ScreenQuad/ScreenQuad.hpp>

#include <string>

class Window {

    private:

        static int windowsCount;

        int windowId;

        GLFWwindow* window;
        int width;
        int height;

        std::string  shaderName;

        void config();

        ScreenQuad* screenQuad;

    public:
        Window() = delete;
        Window(int width,int height);
        ~Window();

        void display();

        int getWidth() { return width;}
        int getHeight() { return height;}

        int getWindowId() { return windowId;}

        void setShaderName(std::string shaderName) {this->shaderName = shaderName;}
};