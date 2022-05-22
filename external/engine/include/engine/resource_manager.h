#pragma once
#include <unordered_map>
#include <string>
#include "shader.h"
#include "texture.h"

template<class Type>
struct Resource
{
	Type obj;
	int instance_count = 1;
	
	Resource(Type o) : obj(o) {}
	Resource() : obj() {}
};

class ResourceManager
{
public:
	// Group --> Resource Name --> Resource
	inline static std::unordered_map<std::string, std::unordered_map<std::string, Resource<Shader>>>	Shaders;
	inline static std::unordered_map<std::string, std::unordered_map<std::string, Resource<Texture2D>>> Textures;

	static Shader& 		LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name, std::string group = "");
	static Texture2D& 	LoadTexture(const char* file, bool alpha, std::string name, std::string group = "");
	static Shader& 		GetShader(std::string name, std::string group = "");
	static Texture2D& 	GetTexture(std::string name, std::string group = "");

	static void DeleteTexture(std::string name, std::string group = "");
	static void DeleteShader(std::string name, std::string group = "");

	static void DeleteGroup(std::string group);
	static void DeleteShaderGroup(std::string group, bool _erase_base = false);
	static void DeleteTextureGroup(std::string group, bool _erase_base = false);

	static void Clear();
private:
	ResourceManager() { }
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const char* file, bool alpha);	
};