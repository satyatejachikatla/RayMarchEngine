#include <GL/glew.h>	// GL Wrangler
#include <GLFW/glfw3.h> // Window management
#include <Infra/glErrors/glErrors.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>

struct WindowEventsData {
    glm::vec3 position;
};

extern struct WindowEventsData windowEventsData;

void window_size_callback(GLFWwindow* window, int width, int height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);