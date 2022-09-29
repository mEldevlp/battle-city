#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
	std::size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
	std::ifstream f;
	std::string file_path = m_path + "\\" + relativeFilePath;

	f.open(file_path, std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		std::cerr << "Fail open file: " << file_path << std::endl;
		return std::string{};
	}
	
	std::stringstream buffer;

	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string shaderName,
	const std::string& vertexPath,
	const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);

	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);

	if (fragmentString.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName,
														std::make_shared<Renderer::ShaderProgram>(vertexString,
																						fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	else
	{
		std::cerr << "Cant load shader program:\n"
			<< "Vertex: " << vertexPath << "\n"
			<< "Fragment: " << fragmentPath << std::endl;

		return nullptr;
	}
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);

	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}

	std::cerr << "Can't find the shader: " << shaderName << std::endl;
	return nullptr;
}
