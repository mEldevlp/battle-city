#include "Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tank.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"


Game::Game(const glm::vec2& windowSize) : m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
    //ResourceManager::getAnimatedSprite("NewAnimSprite")->render();
    if (m_pTank)
    {
        m_pTank->render();
    }
}

void Game::update(const uint64_t delta)
{
    //ResourceManager::getAnimatedSprite("NewAnimSprite")->update(delta);
    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrientation(Tank::EOrientation::TOP);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pTank->setOrientation(Tank::EOrientation::LEFT);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pTank->setOrientation(Tank::EOrientation::RIGHT);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pTank->setOrientation(Tank::EOrientation::BOTTOM);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }

        m_pTank->update(delta);
    }
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("SpriteShader");

    if (!pSpriteShaderProgram)
    {
        std::cerr << "Cant find shader program: " << "SpriteShader" << std::endl;
        return false;
    }

    auto pTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (!pTextureAtlas)
    {
        std::cerr << "Cant find texture atlas: " << "mapTextureAtlas" << std::endl;
        return false;
    }

    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Cant find tanks atlas: " << "" << std::endl;
        return false;
    }


    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimSprite", "mapTextureAtlas", "SpriteShader", 100, 100, "beton");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> waterState;
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));


    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->insertState("eagleState", std::move(eagleState));
    pAnimatedSprite->setState("waterState");


    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);


    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);


    auto pTanksAnimatedSprite = ResourceManager::getAnimatedSprite("TanksAnimatedSprite");
    if (!pTanksAnimatedSprite)
    {
        std::cerr << "Cant find animated sprite: " << "TanksAnimatedSprite" << std::endl;
        return false;
    }

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, .0000001f, glm::vec2(100.f, 100.f));
    return true;
}