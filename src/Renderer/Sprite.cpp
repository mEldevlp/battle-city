#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture,
		const std::string initialSubTexture,
		const std::shared_ptr<ShaderProgram> pShaderProgram)
		: m_pTexture(pTexture)
		, m_pShaderProgram(pShaderProgram)
		, m_lastFrameId(0)
	{
		const GLfloat vertexCoords[] = {
			 0.f, 0.f,
			 0.f, 1.f,
			 1.f, 1.f,
			 1.f, 0.f,
		};

		auto subTexture = m_pTexture->getSubTexture(initialSubTexture);

		const GLfloat textureCoords[] = {

			subTexture.leftBottomUV.x,	subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x,	subTexture.rightTopUV.y,
			subTexture.rightTopUV.x,	subTexture.rightTopUV.y,
			subTexture.rightTopUV.x,	subTexture.leftBottomUV.y,
		};

		const GLint Indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		m_vertexCoordsBuffer.init(vertexCoords, 8 * sizeof(GLfloat)); // second - 2 * 4
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);


		m_textureCoordsBuffer.init(textureCoords, 8 * sizeof(GLfloat)); // second - 2 * 4
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.init(Indices, 6);

		m_vertexArray.unBind();
		m_indexBuffer.unBind();
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer, const size_t frameId) const
	{
		if (m_lastFrameId != frameId)
		{
			m_lastFrameId = frameId;

			const FrameDescription& currentFrameDescription = m_framesDescription[frameId];

			const GLfloat textureCoords[] = {

				currentFrameDescription.leftBottomUV.x,	currentFrameDescription.leftBottomUV.y,
				currentFrameDescription.leftBottomUV.x,	currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x,	currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x,	currentFrameDescription.leftBottomUV.y,
			};

			m_textureCoordsBuffer.update(textureCoords, 8 * sizeof(GLfloat));
		}

		m_pShaderProgram->use();

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
		model = glm::scale(model, glm::vec3(size, 1.f));

		m_pShaderProgram->setFloat("layer", layer);
		m_pShaderProgram->setMatrix4("modelMat", model);



		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	}

	void Sprite::insertFrames(std::vector<FrameDescription> framesDescriptions)
	{
		m_framesDescription = std::move(framesDescriptions);
	}

	double Sprite::getFramesDuration(const size_t frameId) const
	{
		return m_framesDescription[frameId].duration;
	}

	size_t Sprite::getFramesCount() const
	{
		return m_framesDescription.size();
	}
}