#pragma once

#include <Infra/VertexArray/VertexArray.hpp>
#include <Infra/IndexBuffer/IndexBuffer.hpp>
#include <Infra/Shader/Shader.hpp>

#include <GL/glew.h>

class Renderer {
public:
	void Clear() const;
	void Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader,unsigned int mode=GL_TRIANGLES);
};