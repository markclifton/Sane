#include "sane/graphics/framebuffer.hpp"

#include <glad/gl.h>

#include "sane/logging/log.hpp"

namespace Sane
{
    Framebuffer::Framebuffer(int32_t width, int32_t height)
        : size_(width, height)
    {
        Invalidate();
        SANE_INFO("Created framebuffer: {}", framebuffer_);
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteTextures(2, attachments_);
        glDeleteFramebuffers(1, &framebuffer_);
        SANE_INFO("Destroyed framebuffer: {}", framebuffer_);
    }

    void Framebuffer::Resize(int32_t width, int32_t height)
    {
        size_ = { width, height };
        Invalidate();
        SANE_INFO("Resized framebuffer: {}", framebuffer_);
    }

    void Framebuffer::Invalidate()
    {
        if (framebuffer_)
        {
            glDeleteTextures(2, attachments_);
            glDeleteFramebuffers(1, &framebuffer_);
        }

        glGenFramebuffers(1, &framebuffer_);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

        glGenTextures(2, attachments_);

        glBindTexture(GL_TEXTURE_2D, attachments_[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size_.x, size_.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, attachments_[0], 0);

        glBindTexture(GL_TEXTURE_2D, attachments_[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size_.x, size_.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachments_[1], 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        glViewport(0, 0, size_.x, size_.y);
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

    uint32_t Framebuffer::GetAttachment(uint32_t index)
    {
        return attachments_[index];
    }

    const Math::Vec2i Framebuffer::GetSize()
    {
        return size_;
    }
}