#pragma once

#include <glad/glad.h>

namespace Renderer {
	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

		void init(const void* data, const unsigned int sizeData);
		void update(const void* data, const unsigned int sizeData) const;

		void bind() const;
		void unBind() const;

	private:
		GLuint m_id;


	};
}