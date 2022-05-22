#include "engine/resource_manager.h"
#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stb_image.h>
#include <stdexcept>
#include "engine/DebugColors.h"
#include <glad/glad.h>
#include <any>
#include <vector>

Shader& ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name, std::string group)
{
	try
	{
		auto& resource_group = Shaders[group];

		// Create resource or return an already existing one.
		auto result = resource_group.find(name);
		if (result != resource_group.end())
		{
			// Increase resource instance count.
			result->second.instance_count++;
			return result->second.obj;
		}
		else
		{
			resource_group.emplace(name, Resource<Shader>(loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile)));
			return resource_group[name].obj;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("ResourceManager::LoadShader(): " + std::string(e.what()));
	}
}
Shader& ResourceManager::GetShader(std::string name, std::string group)
{
	try
	{
		return Shaders.at(group).at(name).obj;	
	}
	catch(const std::out_of_range& e)
	{
		throw std::out_of_range("ResourceManager::GetShader(): Resource not found. Group = '" + group + "' Name = '" + name + "'");
	}
}
void ResourceManager::DeleteShader(std::string name, std::string group)
{
	try
	{
		Resource<Shader>& res = Shaders.at(group).at(name);
		res.instance_count--;
		if (res.instance_count == 0)
		{
			glDeleteProgram(res.obj.ID);
			Shaders.at(group).erase(name);
		}
	}
	catch(const std::out_of_range& e)
	{
		throw std::out_of_range("ResourceManager::DeleteShader(): Resource not found. Group = '" + group + "' Name = '" + name + "'");
	}
}

Texture2D& ResourceManager::LoadTexture(const char* file, bool alpha, std::string name, std::string group)
{
	try
	{
		auto& resource_group = Textures[group];

		auto result = resource_group.find(name);
		if (result != resource_group.end())
		{
			result->second.instance_count++;
			return result->second.obj;
		}
		else {
			resource_group.emplace(name, Resource<Texture2D>(loadTextureFromFile(file, alpha)));
			return resource_group[name].obj;
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("ResourceManager::LoadTexture(): " + std::string(e.what()));
	}
}
Texture2D& ResourceManager::GetTexture(std::string name, std::string group)
{
	try
	{
		return Textures.at(group).at(name).obj;	
	}
	catch(const std::out_of_range& e)
	{
		throw std::out_of_range("ResourceManager::GetTexture(): Resource not found. Group = '" + group + "' Name = '" + name + "'");
	}
}
void ResourceManager::DeleteTexture(std::string name, std::string group)
{
	try
	{
		Resource<Texture2D>& res = Textures.at(group).at(name);
		res.instance_count--;
		if (res.instance_count == 0)
		{
			glDeleteTextures(1, &res.obj.ID);
			Textures.at(group).erase(name);
		}
	}
	catch(const std::out_of_range& e)
	{
		throw std::out_of_range("ResourceManager::DeleteTexture(): Resource not found. Group = '" + group + "' Name = '" + name + "'");
	}
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);

		if (!vertexShaderFile.is_open())
			throw std::runtime_error("Cannot open vertex shader file '" + std::string(vShaderFile) + "'.");
		if (!fragmentShaderFile.is_open())
			throw std::runtime_error("Cannot open fragment shader file '" + std::string(fShaderFile) + "'.");

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();		
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			if (!geometryShaderFile.is_open())
				throw std::runtime_error("Cannot open geometry shader file '" + std::string(gShaderFile) + "'.");
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception& e)
	{
		throw std::runtime_error(("Failed to read shader files\n" + std::string(e.what())).c_str());
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}
Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_format = GL_RGBA;
		texture.Image_format = GL_RGBA;
	}
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (!data)
		throw std::runtime_error(("Could not load texture '" + std::string(file) + "'.").c_str());
	texture.Generate(width, height, data);
	stbi_image_free(data);
	return texture;
}

void ResourceManager::DeleteShaderGroup(std::string sGroupName, bool _erase_base)
{
	auto& group = Shaders.at(sGroupName);
	std::vector<std::string> names;
	for (auto& pair : group)
		names.push_back(pair.first);
	for (auto& name : names)
		DeleteShader(name, sGroupName);
	if (_erase_base)
		Shaders.erase(sGroupName);
}
void ResourceManager::DeleteTextureGroup(std::string sGroupName, bool _erase_base)
{
	auto& group = Textures.at(sGroupName);
	std::vector<std::string> names;
	for (auto& pair : group)
		names.push_back(pair.first);
	for (auto& name : names)
		DeleteTexture(name, sGroupName);
	if (_erase_base)
		Textures.erase(sGroupName);
}

void ResourceManager::Clear()
{
	for (auto& i : Shaders)
		DeleteShaderGroup(i.first, false);
	for (auto& i : Textures)
		DeleteTextureGroup(i.first, false);

	Shaders.clear();
	Textures.clear();
}

void ResourceManager::DeleteGroup(std::string group)
{
	if (Shaders.find(group) != Shaders.end())
		DeleteShaderGroup(group, true);
	if (Textures.find(group) != Textures.end())
		DeleteTextureGroup(group, true);
}
