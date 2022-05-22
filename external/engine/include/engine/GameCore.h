#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "InputInterface.hpp"
#include "basic_renderer.h"
#include "sprite_renderer.h"
#include "TextRenderer.h"
#include <memory>

class GameCore
{
public:
	bool			Keys[1024];
	bool			KeysProcessed[1024];
	bool			MouseButtons[8];
	bool			MouseButtonsPressed[8];
	unsigned int	Width, Height;
	glm::vec3		BackgroundColor;
	std::string		WindowTitle = "Game";
	bool 			Run = true;
	InputInterface* Input;

	float (*GetTimeFromStart)(void) = NULL;

	GameCore(unsigned int width, unsigned int height);
	virtual ~GameCore();

	virtual void Init();
	virtual void Delete();

	virtual void ProcessInput();
	virtual void ProcessMouse(float xoffset, float yoffset);
	virtual void ProcessScroll(float yoffset);
	virtual void Update(float dt);
	virtual void Render();
	virtual void OnResize();

	void InitEngine();
	void DeleteEngine();
	inline void Exit() { Run = false; }

	static const glm::mat4& GetPixelProjection() { return pixel_projection; }
protected:
	std::shared_ptr<BasicRenderer> basic_renderer;
	std::shared_ptr<SpriteRenderer> sprite_renderer;
	std::shared_ptr<TextRenderer> text_renderer;

	// ===== State variables =====
	// Refresh pixel projection matrix for built-in renderers on window resize event.
	bool refreshPixelProjection = true;

	// Orthographic projection matrix used for transformation of pixel coordinates.
	inline static glm::mat4 pixel_projection = glm::mat4(1.0f);

	// Used for neccessary engine updates. Is separated from OnResize(), because that can be overloaded.
	void _onResize();

	friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
