#pragma once
#include <glad/glad.h>

class Renderbuffer 
{
public:
    unsigned int ID = 0;
    unsigned int Width = 0;
    unsigned int Height = 0;
    // unsigned int Samples = 1; /// TODO

    unsigned int InternalFormat = GL_DEPTH_COMPONENT24;

    Renderbuffer() {}
    ~Renderbuffer() {}

    void Generate(unsigned int width, unsigned int height);
    void Resize(unsigned int new_width, unsigned int new_height);

    void Bind() const;
    void Delete();
};