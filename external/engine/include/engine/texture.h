#pragma once

class Texture2D
{
public:
	unsigned int ID;
	unsigned int Width, Height;
	unsigned int Internal_format;
	unsigned int Image_format;

	unsigned int Wrap_S, Wrap_T;
	unsigned int Filter_min, Filter_mag;

	bool FlipVertically = false;
	bool FlipHorizontally = false;

	unsigned int BindTarget;

	Texture2D();
	~Texture2D();

	void Generate(unsigned int width, unsigned int height, unsigned char* data = 0);
	Texture2D& UpdateParameters();

	Texture2D& SetMagFilter(int filter) { Filter_mag = filter; return *this; }
	Texture2D& SetMinFilter(int filter) { Filter_min = filter; return *this; }
	Texture2D& SetWrapS(int mode) { Wrap_S = mode; return *this; }
	Texture2D& SetWrapT(int mode) { Wrap_T = mode; return *this; }

	Texture2D& Resize(int new_width, int new_height);
	void Delete();

	void Bind() const;
};