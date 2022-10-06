#include "BetonWall.h"

#include "../../Renderer/Sprite.h"
#include "../../Resources/ResourceManager.h"


BetonWall::BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObject(position, size, rotation)
	, m_eCurrentBlockState{ EBlockState::Destroyed,
							EBlockState::Destroyed,
							EBlockState::Destroyed,
							EBlockState::Destroyed, }
	, m_sprite(ResourceManager::getSprite("betonWall"))
	, m_blockOffsets{ 
						glm::vec2(0.f, m_size.y / 2.f),
						glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
						glm::vec2(0.f, 0.f),
						glm::vec2(m_size.x / 2.f, 0.f) }
{
	switch (eBetonWallType)
	{
	case EBetonWallType::All:
		m_eCurrentBlockState.fill(EBlockState::Enabled);
		break;
	case EBetonWallType::Top:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]	= EBlockState::Enabled;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockState::Enabled;
		break;
	case EBetonWallType::Bottom:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)]	= EBlockState::Enabled;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)]	= EBlockState::Enabled;
		break;
	case EBetonWallType::Left:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)]		= EBlockState::Enabled;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)]	= EBlockState::Enabled;
		break;
	case EBetonWallType::Right:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)]		= EBlockState::Enabled;
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)]	= EBlockState::Enabled;
		break;
	case EBetonWallType::TopLeft:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopLeft)] = EBlockState::Enabled;
		break;
	case EBetonWallType::TopRight:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockState::Enabled;
		break;
	case EBetonWallType::BottomLeft:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockState::Enabled;
		break;
	case EBetonWallType::BottomRight:
		m_eCurrentBlockState[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
		break;
	}
}

void BetonWall::renderBlock(const EBlockLocation eBlocklocation) const
{
	static const std::array<glm::vec2, 4> offsets = {
		glm::vec2(0.f, m_size.y / 2.f),
		glm::vec2(m_size.x / 2.f, m_size.y / 2.f),
		glm::vec2(0.f, 0.f),
		glm::vec2(m_size.x / 2.f, 0.f),
	};

	if (const EBlockState state = m_eCurrentBlockState[static_cast<size_t>(eBlocklocation)]; state != EBlockState::Destroyed)
	{
		m_sprite->render(m_position + m_blockOffsets[static_cast<size_t>(eBlocklocation)], m_size / 2.f, m_rotation);
	}
}


void BetonWall::render() const
{
	renderBlock(EBlockLocation::BottomLeft);
	renderBlock(EBlockLocation::BottomRight);
	renderBlock(EBlockLocation::TopLeft);
	renderBlock(EBlockLocation::TopRight);
}

void BetonWall::update(const uint64_t delta){}