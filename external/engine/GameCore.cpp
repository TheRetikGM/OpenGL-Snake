#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/GameCore.h"
#include "engine/resource_manager.h"
#include "engine_config.h"
#define RESOURCE_GROUP "__engine"

GameCore::GameCore(unsigned int width, unsigned int height) 
	: Width(width), Height(height)
	, Keys(), KeysProcessed()
	, BackgroundColor(0.0f)
	, Input(new InputInterface(Keys, KeysProcessed, MouseButtons, MouseButtonsPressed))
{
}
GameCore::~GameCore()
{	
	delete Input;
}
void GameCore::Delete()
{
}
void GameCore::OnResize()
{
}
void GameCore::ProcessMouse(float xoffset, float yoffset)
{
}
void GameCore::ProcessScroll(float yoffset)
{
}
void GameCore::Init()
{

}
void GameCore::DeleteEngine()
{
	// Delete everything that was not deleted along with engine stuff.
	ResourceManager::Clear();
}
void GameCore::InitEngine()
{
	pixel_projection = glm::ortho(0.0f, float(Width), float(Height), 0.0f, 0.0f, -1.0f);
	Shader& basic = ResourceManager::LoadShader(ENGINE_SHADERS_DIR "BasicRender.vert", ENGINE_SHADERS_DIR "BasicRender.frag", nullptr, "basic", RESOURCE_GROUP);
	Shader& sprite = ResourceManager::LoadShader(ENGINE_SHADERS_DIR "SpriteRender.vert", ENGINE_SHADERS_DIR "SpriteRender.frag", nullptr, "sprite", RESOURCE_GROUP);
	Shader& normal = ResourceManager::LoadShader(ENGINE_SHADERS_DIR "normals.vert", ENGINE_SHADERS_DIR "normals.frag", nullptr, "normal", RESOURCE_GROUP);

	// Set projection.
	basic.Use().SetMat4("projection", pixel_projection);
	sprite.Use().SetMat4("projection", pixel_projection);

	// Create renderers.
	basic_renderer = std::make_shared<BasicRenderer>(basic);
	sprite_renderer = std::make_shared<SpriteRenderer>(sprite);
	text_renderer = std::make_shared<TextRenderer>(Width, Height);
}
void GameCore::ProcessInput()
{
	if (Input->Pressed(Key::ESCAPE))
		Run = false;
}
void GameCore::Update(float dt)
{	
}
void GameCore::Render()
{
}
void GameCore::_onResize()
{
	if (refreshPixelProjection)
	{
		pixel_projection = glm::ortho(0.0f, float(Width), float(Height), 0.0f, 0.0f, -1.0f);
		basic_renderer->GetShader().Use().SetMat4("projection", pixel_projection);
		sprite_renderer->GetShader().Use().SetMat4("projection", pixel_projection);
		text_renderer->SetProjection(pixel_projection);
	}
}
