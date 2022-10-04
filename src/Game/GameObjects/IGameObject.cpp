#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2& position, const glm::vec2& size, const float rotation)
	: m_position(position)
	, m_size(size)
	, m_rotation(rotation)
{

}

IGameObject::~IGameObject()
{

}

void IGameObject::render() const
{

}

void IGameObject::update(const uint64_t delta)
{

}
