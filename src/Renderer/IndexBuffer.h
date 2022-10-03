#pragma once

#include <glad/glad.h>

namespace Renderer {
	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		void init(const void* data, const unsigned int sizeData);
		void bind() const;
		void unBind() const;

	private:
		GLuint m_id;


	};
}