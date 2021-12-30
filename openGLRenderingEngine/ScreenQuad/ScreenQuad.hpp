#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include <Infra/VertexArray/VertexArray.hpp>
#include <Infra/VertexBuffer/VertexBuffer.hpp>
#include <Infra/IndexBuffer/IndexBuffer.hpp>
#include <Infra/Shader/Shader.hpp>
#include <Infra/Texture/Texture.hpp>
#include <Infra/Renderer/Renderer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ScreenQuad {
	public:
		ScreenQuad() = delete;
		ScreenQuad(int width, int height,std::string shader_name, GLFWwindow* window);
		~ScreenQuad();

		void OnUpdate(float deltaTime);
		void OnRender();

	private:
		int width;
		int height;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		std::unique_ptr<Shader> m_Shader;
		
		std::unique_ptr<Texture> m_Texture[5];

		Renderer m_Renderer;

		GLFWwindow* window;

		std::string shader_name;
		std::string texture_name[5];

		glm::mat4 m_Proj;
};

