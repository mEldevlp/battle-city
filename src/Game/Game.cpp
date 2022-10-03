#include "Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    ResourceManager::getAnimatedSprite("NewAnimSprite")->render();
}

void Game::update(const uint64_t delta)
{

    ResourceManager::getAnimatedSprite("NewAnimSprite")->update(delta);

}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

const char* vertexShader = "res\\shaders\\vertex.vert";
const char* fragmentShader = "res\\shaders\\fragment.frag";
const char* vSpriteShader = "res\\shaders\\vSprite.vert";
const char* fSpriteShader = "res\\shaders\\fSprite.frag";
const char* map16x16 = "res/textures/map_16x16.png";

bool Game::init()
{
    auto pDefaultSHaderProgram = ResourceManager::loadShaders("DefaultShader", vertexShader, fragmentShader);

    if (!pDefaultSHaderProgram)
    {
        std::cerr << "Cant create shader program: " << "DefautShader" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", vSpriteShader, fSpriteShader);

    if (!pSpriteShaderProgram)
    {
        std::cerr << "Cant create shader program: " << "SpriteShader" << std::endl;
        return false;
    }


    auto tex = ResourceManager::loadTexture("DefaultTexture", map16x16);


    std::vector<std::string> subTexturesNames = {
        "block",
        "topBlock",
        "bottomBlock",
        "leftBlock",
        "rightBlock",
        "topLeftBlock",
        "topRightBlock",
        "bottomLeftBlock",
        "bottomRightBlock",

        "beton",
        "topBeton",
        "bottomBeton",
        "leftBeton",
        "rightBeton",
        "topLeftBeton",
        "topRightBeton",
        "bottomLeftBeton",
        "bottomRightBeton",

        "water1",
        "water2",
        "water3",
        "trees",
        "ice",
        "wall",

        "eagle",
        "deadEagle",
        "nothing",
        "respawn1",
        "respawn2",
        "respawn3",
        "respawn4",
    };


    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", map16x16, std::move(subTexturesNames), 16, 16);

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
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


    pDefaultSHaderProgram->use();
    pDefaultSHaderProgram->setInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 200.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pDefaultSHaderProgram->setMatrix4("projectionMat", projectionMatrix);


    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    return true;
}