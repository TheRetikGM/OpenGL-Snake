#pragma once
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>

class SpriteRenderer
{
public:
	SpriteRenderer(Shader shader);
	~SpriteRenderer();

	SpriteRenderer& RenderSprite(const Texture2D& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));

	// Draws only part of the texture.
	SpriteRenderer& RenderPartialSprite(const Texture2D& texture
						 , glm::vec2 vPartOffset	// Part offset in pixels
						 , glm::vec2 vPartSize		// part size in pixels
						 , glm::vec2 vPosition		// Pozition in pixels
						 , glm::vec2 vSize			// Wanted size in pixels
						 , float fRotate = 0.0f
						 , glm::vec3 vColor = glm::vec3(1.0f)
	);

	SpriteRenderer& RenderGLTexture(unsigned int ID, glm::vec2 vPosition, glm::vec2 vSize, float fRotate, glm::vec3 vColor, bool bFlipH = false, bool bFlipV = false);

	Shader GetShader() const { return shader; };
	SpriteRenderer& ForceColor(bool b) { shader.SetInt("force_color", int(b), true); return *this; }
private:
	Shader shader;
	unsigned int quadVAO;

	void initRenderData();
};