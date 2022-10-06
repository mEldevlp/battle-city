#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <memory>
#include <string>


namespace RenderEngine {

class Texture2D;
class ShaderProgram;

class Sprite {
public:

	struct FrameDescription {
		FrameDescription(const glm::vec2 a_leftBottomUV, const glm::vec2 a_rightTopUV, const uint64_t a_duration)
			: leftBottomUV(a_leftBottomUV)
			, rightTopUV(a_rightTopUV)
			, duration(a_duration){}
		glm::vec2 leftBottomUV;
		glm::vec2 rightTopUV;
		uint64_t duration;
	};

	Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram);

	~Sprite();

	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;

	//virtual void render(const glm::vec2& position, const glm::vec2& size, 
	//	const float rotation, const float layer = 0.f, const size_t frameId = 0) const;

	void render(const glm::vec2& position,
		const glm::vec2& size,
		const float rotation,
		const float layer = 0.f,
		const size_t frameId = 0) const;

	void insertFrames(std::vector<FrameDescription> framesDescriptions);
	 uint64_t getFramesDuration(const size_t frameId) const;
	 size_t getFramesCount() const;

protected:

	std::shared_ptr<Texture2D> m_pTexture;
	std::shared_ptr<ShaderProgram> m_pShaderProgram;

	VertexArray m_vertexArray;
	VertexBuffer m_vertexCoordsBuffer;
	VertexBuffer m_textureCoordsBuffer;
	IndexBuffer m_indexBuffer;

	std::vector<FrameDescription> m_framesDescription;
	mutable size_t m_lastFrameId;

};
}