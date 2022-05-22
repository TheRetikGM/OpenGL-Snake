#include "engine/Renderbuffer.h"

void Renderbuffer::Generate(unsigned int width, unsigned int height)
{
    this->Width = width;
    this->Height = height;
    glGenRenderbuffers(1, &ID);
    glBindRenderbuffer(GL_RENDERBUFFER, ID);
    glRenderbufferStorage(GL_RENDERBUFFER, InternalFormat, Width, Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
void Renderbuffer::Resize(unsigned int new_width, unsigned int new_height)
{
    glDeleteRenderbuffers(1, &ID);
    Generate(new_width, new_height);
}
void Renderbuffer::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, ID);
}
void Renderbuffer::Delete()
{
    glDeleteRenderbuffers(1, &ID);
}