#pragma once

#include "IGameObject.h"


#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class Eagle : public IGameObject {

public:
	enum class EEagleState : uint8_t {
		Alive = 0,
		Dead
	};

	Eagle(const glm::vec2&, const glm::vec2&, const float);
	virtual void render() const override;
	void update(const uint64_t) override;

private:
	std::array<std::shared_ptr<RenderEngine::Sprite> , 2> m_sprite;
	EEagleState m_eCurrentState;

};
