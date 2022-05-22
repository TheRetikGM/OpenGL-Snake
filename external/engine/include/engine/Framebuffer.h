#pragma once
#include "engine/texture.h"
#include <vector>
#include <variant>
#include <glad/glad.h>
#include "engine/Renderbuffer.h"
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include "engine/Core.h"
#include <string>

enum class FramebufferInternalFormats: unsigned int {
    RGBA = GL_RGBA,
    RGB = GL_RGB,
    RED = GL_RED,
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    STENCIL_INDEX8 = GL_STENCIL_INDEX8
};
enum class StorageType: unsigned int {
    TEXTURE = 0,
    RENDERBUFFER = 1
};

struct FramebufferAttachment
{
    StorageType storageType = StorageType::TEXTURE;
    FramebufferInternalFormats internalFormat = FramebufferInternalFormats::RGB;
    std::variant<Texture2D, Renderbuffer> storage;
    int nColorAttachment = -1;
    unsigned int bufferID = 0;
    
    FramebufferAttachment() = default;

    // Attaches this attachment to the currently bound framebuffer object.
    void Attach(int nColorAttachment = -1);
    void Resize(unsigned int new_width, unsigned int new_height);
    void Delete();
    void Generate(unsigned int width, unsigned int height);
    void UpdateID();
    glm::uvec2 GetSize();

    template<class T>
    T& GetStorage() { return std::get<T>(storage); }
};

class Framebuffer
{
public:
    unsigned int ID = 0;
    std::vector<sptr_t<FramebufferAttachment>> Attachments;
    bool PreserveFBOBinding = true;

    glm::uvec2 Size = glm::uvec2(0, 0);
    unsigned int& Width = Size.x;
    unsigned int& Height = Size.y;

    Framebuffer();

    Framebuffer& AddAttachment(sptr_t<FramebufferAttachment> attachment);
    Framebuffer& Generate(unsigned int width, unsigned int height);
    Framebuffer& Bind();
    Framebuffer& Unbind();
    Framebuffer& Clear(glm::vec3 vClearColor = glm::vec3(0.0f));
    Framebuffer& Resize(unsigned int new_width, unsigned int new_height);
    unsigned int GetColorAttachmentID(int nColorAttachment) { return *mColorBufferBindings[nColorAttachment]; }
    void Delete();
    // Delete and recreate framebuffer.
    void Invalidate();

    unsigned int GetMaxColorAttachments() { return nMaxAttachments; }

    // Factory pattern
    /// Basic framebuffer with ONE color attachment + DepthStencil attachemnt.
    static sptr_t<Framebuffer> CreateBasicFramebuffer(unsigned int width, unsigned int height);
    /// Basic framebuffer with one Texture2D color attachment. No depth or stencil buffer is created.
    static sptr_t<Framebuffer> Create2DBasicFramebuffer(unsigned int width, unsigned int height);

private:
    unsigned int nFbBackupID = 0;
    unsigned int nMaxAttachments = -1;
    std::string sError = "";
    std::unordered_map<int, unsigned int*> mColorBufferBindings;

    // 1/ Atleast ONE color attachment.
    // 2/ All attachments have the same size as Size.
    // 3/ Number of attachments is within nMaxAttachments.
    bool checkFramebufferValidity(glm::uvec2 size);
};