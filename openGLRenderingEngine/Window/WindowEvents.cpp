#include <Window/WindowsEvents.hpp>

#include <iostream>

using namespace std;

WindowEventsData windowEventsData;

#define POS_SPEED 0.001

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glCall(glViewport(0, 0, width, height));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	static map<unsigned int,bool> keymap;

	if(action == GLFW_PRESS) {
		keymap[key] = true;
	} else if(action == GLFW_RELEASE){
		keymap[key] = false;
	}

	static glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);

	if (keymap[GLFW_KEY_A])
			position.x += -POS_SPEED;
	if (keymap[GLFW_KEY_D])
			position.x += POS_SPEED;
	if (keymap[GLFW_KEY_W])
			position.z += POS_SPEED;
	if (keymap[GLFW_KEY_S])
			position.z += -POS_SPEED;
	if (keymap[GLFW_KEY_SPACE] || keymap[GLFW_KEY_X])
			position.y += POS_SPEED;
	if (keymap[GLFW_KEY_LEFT_ALT] || keymap[GLFW_KEY_Z] )
			position.y += -POS_SPEED;
	
	windowEventsData.position = position;
	
	// cout << position.x << " " << position.y << " " << position.z << endl;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

