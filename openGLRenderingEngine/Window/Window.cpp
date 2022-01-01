#include <Window/Window.hpp>

#include <iostream>
#include <stdexcept>
#include <Infra/glErrors/glErrors.hpp>
#include <Window/WindowsEvents.hpp>

using namespace std;


Window::Window(int width,int height) : width(width) , height(height){
	config();
}

Window::~Window(){
	glCall(glfwTerminate());
}

void Window::config() {

	/* Initialize the library */
	if (!glfwInit()){
        runtime_error("Failed to init glfw");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "OpenglWindow", NULL, NULL);
	if (!window) {
		glfwTerminate();
        runtime_error("Failed to create glfw window");
	}

	/* Make the window's interaction callbacks */
	glCall(glfwMakeContextCurrent(window));

	/* Vsyncing screen number for proper frame rate*/
	glCall(glfwSwapInterval(1));

	/* glewInit after context setting */
	if (glewInit() != GLEW_OK) {
		runtime_error("Failed to init glew");
	}

	// OpenGL Display Settings
	glCall(glEnable(GL_BLEND));
	glCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
	glCall(glEnable(GL_DEPTH_TEST));

	// TODO: Need to add callbacks for windows

	glCall(glfwSetCursorPosCallback(window, cursor_position_callback));
	glCall(glfwSetMouseButtonCallback(window, mouse_button_callback));
	glCall(glfwSetScrollCallback(window, scroll_callback));

	/* Set Input Key Callback */
	glCall(glfwSetKeyCallback(window, key_callback));

	/*Window Resize*/
	glCall(glfwSetWindowSizeCallback(window, window_size_callback));

	/* OpenGl Version */
	cout << "OpenGL Version : " << glGetString(GL_VERSION) << endl;    
}

void Window::display() {

		screenQuad = new ScreenQuad(width,height,"./shader/Test.shader",window);

		while (!glfwWindowShouldClose(window)) {

			glCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
			glCall(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));

			if(screenQuad) {
				screenQuad->OnUpdate(0.0f);
				screenQuad->OnRender();
			}

			glCall(glfwSwapBuffers(window));
			glCall(glfwPollEvents());

		}

		delete screenQuad;

}

