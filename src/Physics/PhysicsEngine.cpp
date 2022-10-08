#include "PhysicsEngine.h"
#include "../Game/GameObjects/IGameObject.h"
#include "../Game/Level.h"

namespace Physics {

	std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::m_dynamicObjects;
	std::shared_ptr<Level> PhysicsEngine::m_pCurrentLevel;

	void PhysicsEngine::init()
	{

	}

	void PhysicsEngine::terminate()
	{
		m_dynamicObjects.clear();
		m_pCurrentLevel.reset();
	}

	void PhysicsEngine::update(const double tickrate)
	{
		for (auto& currentObject : m_dynamicObjects)
		{
			if (currentObject->getCurrentVelocity() > 0)
			{
				if (currentObject->getCurrentDirection().x != 0.f)
				{
					currentObject->getCurrentPosition() = glm::vec2(currentObject->getCurrentPosition().x,
						static_cast<unsigned int>(currentObject->getCurrentPosition().y / 4.f + 0.5f) * 4);
				}
				else if (currentObject->getCurrentDirection().y != 0.f)
				{
					currentObject->getCurrentPosition() = glm::vec2(static_cast<unsigned int>(currentObject->getCurrentPosition().x / 4.f + 0.5f) * 4,
						currentObject->getCurrentPosition().y);
				}


				const auto newPos = currentObject->getCurrentPosition() + currentObject->getCurrentDirection() * static_cast<float>(currentObject->getCurrentVelocity() * tickrate);
				const auto& colliders = currentObject->getColliders();
				std::vector<std::shared_ptr<IGameObject>> objectToCheck = m_pCurrentLevel->getObjectsInArea(newPos, newPos + currentObject->getSize());

				bool hasCollision = false;

				for (const auto& currentObjectToCheck : objectToCheck)
				{
					const auto& collidersToCheck = currentObjectToCheck->getColliders();
					
					if (!collidersToCheck.empty())
					{
						if (hasIntersection(colliders, newPos,  collidersToCheck, currentObjectToCheck->getCurrentPosition()))
						{
							hasCollision = true;
							break;
						}
					}

				}

				if (!hasCollision)
				{
					currentObject->getCurrentPosition() = newPos;
				}
				else
				{
					if (currentObject->getCurrentDirection().x != 0.f)
					{
						currentObject->getCurrentPosition() = glm::vec2(currentObject->getCurrentPosition().x,
							static_cast<unsigned int>(currentObject->getCurrentPosition().y / 8.f + 0.5f) * 8);
					}
					else if (currentObject->getCurrentDirection().y != 0.f)
					{
						currentObject->getCurrentPosition() = glm::vec2(static_cast<unsigned int>(currentObject->getCurrentPosition().x / 8.f + 0.5f) * 8,
							currentObject->getCurrentPosition().y);
					}
				}

				
			}
		}
	}

	void PhysicsEngine::addDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
	{
		m_dynamicObjects.insert(std::move(pGameObject));
	}

	bool PhysicsEngine::hasIntersection(const std::vector<AABB>& colliders1, const glm::vec2& position1,
								const std::vector<AABB>& colliders2, const glm::vec2& position2)
	{
		for (const auto& currentCollider1 : colliders1)
		{
			const glm::vec2 currentCollider1_bottomLeft_world = currentCollider1.bottomLeft + position1;
			const glm::vec2 currentCollider1_topRight_world   = currentCollider1.topRight + position1;
			for (const auto& currentCollider2 : colliders2)
			{
				const glm::vec2 currentCollider2_bottomLeft_world = currentCollider2.bottomLeft + position2;
				const glm::vec2 currentCollider2_topRight_world   = currentCollider2.topRight + position2;

				if (currentCollider1_bottomLeft_world.x >= currentCollider2_topRight_world.x)
				{
					return false;
				}
				if (currentCollider1_topRight_world.x <= currentCollider2_bottomLeft_world.x)
				{
					return false;
				}

				if (currentCollider1_bottomLeft_world.y >= currentCollider2_topRight_world.y)
				{
					return false;
				}
				if (currentCollider1_topRight_world.y <= currentCollider2_bottomLeft_world.y)
				{
					return false;
				}
			}
		}
		return true;
	}


	void PhysicsEngine::setCurrentLevel(std::shared_ptr<Level> pLevel)
	{
		m_pCurrentLevel.swap(pLevel);
	}
}