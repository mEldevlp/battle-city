#pragma once

#include <memory>
#include <glm/vec2.hpp>

namespace Renderer {
	class AnimatedSprite;
}

class Tank {
public:
	enum class EOrientation {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position);

	void render() const;
	void setOrientation(const EOrientation eOrientation);
	void move(const bool move);
	void update(const uint64_t delta);


private:
	EOrientation m_eOrientation;
	std::shared_ptr<Renderer::AnimatedSprite> m_pSprite;
	bool m_move;
	float m_velocity;
	glm::vec2 m_position;
	glm::vec2 m_moveOffset;


};