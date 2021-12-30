#pragma once

#include <Infra/glErrors/glErrors.hpp>
#include <Infra/VertexBuffer/VertexBuffer.hpp>
#include <Infra/VertexBufferLayout/VertexBufferLayout.hpp>

class VertexArray {
	private:
		unsigned int m_RendererID;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;
};