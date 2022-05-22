#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projection, model;
uniform bvec2 inverse_tex;

void main()
{
	TexCoords = vertex.zw;

	if (inverse_tex.x)
		TexCoords.x = 1.0 - TexCoords.x;
	if (inverse_tex.y)
		TexCoords.y = 1.0 - TexCoords.y;

	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}