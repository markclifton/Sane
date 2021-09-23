#include "sane/graphics/framebuffer.hpp"

namespace Sane
{
    Framebuffer::Framebuffer(size_t Width, size_t Height)
        : size({ Width, Height })
    {
        Invalidate();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteTextures(2, attachments);
        glDeleteFramebuffers(1, &framebuffer);
    }

    void Framebuffer::Resize(size_t Width, size_t Height)
    {
        size = { Width, Height };
        Invalidate();
    }

    void Framebuffer::Invalidate()
    {
        if (framebuffer)
        {
            glDeleteTextures(2, attachments);
            glDeleteFramebuffers(1, &framebuffer);
        }

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(2, attachments);

        glBindTexture(GL_TEXTURE_2D, attachments[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, attachments[0], 0);

        glBindTexture(GL_TEXTURE_2D, attachments[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachments[1], 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
    }

    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Clear()
    {
        glClearColor(.05f, .06f, .11f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    GLuint Framebuffer::GetAttachment(size_t index)
    {
        return attachments[index];
    }

    glm::vec2 Framebuffer::GetSize()
    {
        return size;
    }
}