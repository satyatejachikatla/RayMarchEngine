#include <ScreenQuad/ScreenQuad.hpp>
#include <Infra/glErrors/glErrors.hpp>

using namespace std;

ScreenQuad::ScreenQuad(int width, int height,string shader_name, GLFWwindow* window) : width(width), height(height), shader_name(shader_name), window(window) {
	
	m_Proj = glm::ortho(0.0f,float(width),0.0f,float(height),-1.0f,1.0f);

	float positions[] = {
		0.0f, 0.0f,
		0.0f, (float)height,
		(float)width, (float)height,
		(float)width, 0.0f,
	};

	unsigned int indices[] = {
		0,1,2,
		0,2,3
	};

	glCall(glEnable(GL_BLEND));
	glCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

	m_VAO = make_unique<VertexArray>();
	m_VBO = make_unique<VertexBuffer>(positions,sizeof(positions));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VBO,layout);
	m_IndexBuffer = make_unique<IndexBuffer>(indices,6);


	// TODO : Enable Texture

	// for(int i=0;i<5;i++){
	// 	texture_name[i] = string("./res/textures/gradient.jpg");
	// 	m_Texture[i] = make_unique<Texture>(texture_name[i].c_str());
	// 	m_Texture[i]->Bind(i);
	// }

	m_Shader = make_unique<Shader>(shader_name.c_str());

}
ScreenQuad::~ScreenQuad() {

}

void ScreenQuad::OnUpdate(float deltaTime) {
	
}

void ScreenQuad::OnRender() {
	static float itter = 0;
	glCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
	glCall(glClear(GL_COLOR_BUFFER_BIT));

	double xpos, ypos;
	glCall(glfwGetCursorPos(window, &xpos, &ypos));

	
	// if(m_Shader->isLoadFailed()|| glfwGetKey(window,GLFW_KEY_ENTER) == GLFW_PRESS){
		
	// 	itter = 0;
	// 	m_Shader.reset();
	// 	m_Shader = make_unique<Shader>(shader_name.c_str());

	// 	for(int i=0;i<5;i++){
	// 		m_Texture[i].reset();
	// 		m_Texture[i] = make_unique<Texture>(texture_name[i].c_str());
	// 		m_Texture[i]->Bind(i);
	// 	}
	// } else {
		
		// for(int i=0;i<5;i++)
		// 	m_Texture[i]->Bind(i);
		m_Shader->Bind();

		m_Shader->SetUniformMat4f("u_P",m_Proj);
		m_Shader->SetUniform1f("u_Time",glfwGetTime());
		m_Shader->SetUniform1f("u_Itteration",itter++);
		m_Shader->SetUniformVec2f("u_Resolution",glm::vec2(width,height));
		m_Shader->SetUniformVec2f("u_Mouse",glm::vec2(xpos,ypos));
		m_Shader->SetUniform1i("u_TextureChannels[0]",0);
		m_Shader->SetUniform1i("u_TextureChannels[1]",1);
		m_Shader->SetUniform1i("u_TextureChannels[2]",2);
		m_Shader->SetUniform1i("u_TextureChannels[3]",3);
		m_Shader->SetUniform1i("u_TextureChannels[4]",4);

		m_Renderer.Draw(*m_VAO,*m_IndexBuffer,*m_Shader);
	// }

}
