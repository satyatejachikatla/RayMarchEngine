#include <Infra/glErrors/glErrors.hpp>
#include <Infra/Renderer/Renderer.hpp>

#include <GL/glew.h>
#include <iostream>

void Renderer::Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader,unsigned int mode) {
	
	//Shader Bind//
	shader.Bind();
	//VAO Bind//
	va.Bind();
	// Index Bind //
	ib.Bind();

	glCall(glDrawElements(mode,ib.GetCount(),GL_UNSIGNED_INT,nullptr));

}
void Renderer::Clear() const{
	glCall(glClear(GL_COLOR_BUFFER_BIT));
}