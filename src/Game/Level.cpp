#include "Level.h"

#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWall.h"
#include "GameObjects/Trees.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"

#include <iostream>
#include <algorithm>
#include <cmath>


std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation)
{
    switch (description)
    {
    case '0':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Right, position, size, rotation, 0.f);
    case '1':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Bottom, position, size, rotation, 0.f);
    case '2':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Left, position, size, rotation, 0.f);
    case '3':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Top, position, size, rotation, 0.f);
    case '4':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::All, position, size, rotation, 0.f);
    case 'G':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomLeft, position, size, rotation, 0.f);
    case 'H':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomRight, position, size, rotation, 0.f);
    case 'I':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopLeft, position, size, rotation, 0.f);
    case 'J':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopRight, position, size, rotation, 0.f);
    case '5':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Right, position, size, rotation, 0.f);
    case '6':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Bottom, position, size, rotation, 0.f);
    case '7':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Left, position, size, rotation, 0.f);
    case '8':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Top, position, size, rotation, 0.f);
    case '9':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::All, position, size, rotation, 0.f);
    case 'A':
        return std::make_shared<Water>(position, size, rotation, 0.f);
    case 'B':
        return std::make_shared<Trees>(position, size, rotation, 1.f);
    case 'C':
        return std::make_shared<Ice>(position, size, rotation, -1.f);
    case 'E':
        return std::make_shared<Eagle>(position, size, rotation, 0.f);
    case 'D':
        return nullptr;
	default: std::cerr << "Unknown Game object: " << description << std::endl;
	}

	return nullptr;
}

Level::Level(const std::vector<std::string>& levelDescription)
{
	if (levelDescription.empty())
	{
		std::cerr << "Empty level description!" << std::endl;
	}

    m_widthBlocks  = levelDescription[0].length(); // cols
    m_heightBlocks = levelDescription.size();		// rows

    m_widthPixels  = static_cast<unsigned int> (m_widthBlocks * BLOCK_SIZE);
    m_heightPixels = static_cast<unsigned int> (m_heightBlocks * BLOCK_SIZE);


	m_mapObjects.reserve(m_widthBlocks * m_heightBlocks + 4);

    m_playerRespawn_1   = { BLOCK_SIZE * (m_widthBlocks / 2.f - 1.f),   BLOCK_SIZE / 2.f };
    m_playerRespawn_2   = { BLOCK_SIZE * (m_widthBlocks / 2.f + 3.f),   BLOCK_SIZE / 2.f };

    m_enemyRespawn_1    = {  BLOCK_SIZE,                                BLOCK_SIZE * m_heightBlocks - BLOCK_SIZE  / 2.f};
    m_enemyRespawn_2    = {  BLOCK_SIZE * (m_widthBlocks / 2.f + 1.f),  BLOCK_SIZE * m_heightBlocks - BLOCK_SIZE  / 2.f};
    m_enemyRespawn_3    = {  BLOCK_SIZE * m_widthBlocks,                BLOCK_SIZE * m_heightBlocks - BLOCK_SIZE  / 2.f};


    size_t currentBottomOffset = static_cast<size_t>(BLOCK_SIZE * (m_heightBlocks - 1)) + BLOCK_SIZE / 2.f;

	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;

		for (const char currentElement : currentRow)
		{
            switch (currentElement)
            {
            case 'K':
                m_playerRespawn_1 = { currentLeftOffset, currentBottomOffset };
                m_mapObjects.emplace_back(nullptr);
                break;
            case 'L':
                m_playerRespawn_2 = { currentLeftOffset, currentBottomOffset };
                m_mapObjects.emplace_back(nullptr);
                break;
            case 'N':
                m_enemyRespawn_1 = { currentLeftOffset, currentBottomOffset };
                m_mapObjects.emplace_back(nullptr);
                break;
            case 'M':
                m_enemyRespawn_2 = { currentLeftOffset, currentBottomOffset };
                m_mapObjects.emplace_back(nullptr);
                break;
            case 'O':
                m_enemyRespawn_3 = { currentLeftOffset, currentBottomOffset };
                m_mapObjects.emplace_back(nullptr);
                break;


            default:
                m_mapObjects.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
            }


			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}

    // bottom border
    m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(m_widthBlocks * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // top border
    m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, m_heightBlocks * BLOCK_SIZE + BLOCK_SIZE / 2.f), glm::vec2(m_widthBlocks * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // left border
    m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

    // right border
    m_mapObjects.emplace_back(std::make_shared<Border>(glm::vec2((m_widthBlocks + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (m_heightBlocks + 1) * BLOCK_SIZE), 0.f, 0.f));

}

void Level::render() const
{
	for (const auto& currentMapObject : m_mapObjects)
	{
		if (currentMapObject)
		{
			currentMapObject->render();
		}
	}
}

void Level::update(const double delta)
{
	for (const auto& currentMapObject : m_mapObjects)
	{
		if (currentMapObject)
		{
			currentMapObject->update(delta);
		}
	}
}

size_t Level::getLevelWidth() const
{
    return (m_widthBlocks + 3) * BLOCK_SIZE;
}

size_t Level::getLevelHeight() const
{
    return (m_heightBlocks + 1) * BLOCK_SIZE;
}


std::vector<std::shared_ptr<IGameObject>> Level::getObjectsInArea(const glm::vec2& bottomLeft, const glm::vec2& rightTop)
{

    system("cls");
    std::vector<std::shared_ptr<IGameObject>> output;
    output.reserve(9);

    glm::vec2 bottomLeft_coverted(std::clamp(bottomLeft.x - BLOCK_SIZE,                         0.f, static_cast<float>(m_widthPixels)),
                                  std::clamp(m_heightPixels - bottomLeft.y + BLOCK_SIZE / 2.f,  0.f, static_cast<float>(m_heightPixels)));

    glm::vec2 rightTop_coverted(std::clamp(rightTop.x - BLOCK_SIZE,                        0.f, static_cast<float>(m_widthPixels)),
                                std::clamp(m_heightPixels - rightTop.y + BLOCK_SIZE / 2.f, 0.f, static_cast<float>(m_heightPixels)));

    std::cout << "Position: \n" << "X: " << bottomLeft_coverted.x + BLOCK_SIZE / 2.f << "\nY: " << bottomLeft_coverted.y + BLOCK_SIZE / 2.f << std::endl;


    size_t startX = static_cast<size_t>(floor(bottomLeft_coverted.x / BLOCK_SIZE));
    size_t endX   = static_cast<size_t>(ceil(rightTop_coverted.x    / BLOCK_SIZE));
    
    size_t startY = static_cast<size_t>(floor(rightTop_coverted.y  / BLOCK_SIZE));
    size_t endY   = static_cast<size_t>(ceil(bottomLeft_coverted.y / BLOCK_SIZE));
    
    for (size_t currentColumn = startX; currentColumn < endX; ++currentColumn)
    {
        for (size_t currentRow = startY; currentRow < endY; ++currentRow)
        {
            auto& currentObject = m_mapObjects[currentRow * m_widthBlocks + currentColumn];
            
            if (currentObject)
            {
                output.push_back(currentObject);
            }
        }
    }

    if (endX >= m_widthBlocks)
    {
        output.push_back(m_mapObjects[m_mapObjects.size() - 1]);
    }
    if (startX <= 1)
    {
        output.push_back(m_mapObjects[m_mapObjects.size() - 2]);
    }
    if (startY <= 1)
    {
        output.push_back(m_mapObjects[m_mapObjects.size() - 3]);
    }
    if (endY >= m_heightBlocks)
    {
        output.push_back(m_mapObjects[m_mapObjects.size() - 4]);
    }

    
    return output;
}