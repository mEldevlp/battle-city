#pragma once

#include "IGameObject.h"


#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class BetonWall : public IGameObject {

public:
	enum class EBetonWallType {
		All,
		Top,
		Bottom,
		Left,
		Right,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};
	
	enum class EBlockState {
		Enabled = 0,
		Destroyed
	};

	enum class EBlockLocation {
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};


	BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation);
	virtual void render() const override;
	virtual void update(const uint64_t delta) override;

private:
	void renderBlock(const EBlockLocation eBricklocation) const;

	std::shared_ptr<RenderEngine::Sprite> m_sprite;
	std::array<EBlockState, 4> m_eCurrentBlockState;

	std::array<glm::vec2, 4> m_blockOffsets;

};