#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace Sane
{
    class Framebuffer
    {
    public:
        Framebuffer(size_t Width, size_t Height);
        ~Framebuffer();

        void Resize(size_t Width, size_t Height);
        void Invalidate();
        void Bind();
        void Unbind();
        void Clear();

        GLuint GetAttachment(size_t index);
        glm::vec2 GetSize();

    private:
        glm::vec2 size{ 0, 0 };
        GLuint framebuffer{ 0 };
        GLuint attachments[2]{ 0,0 };
    };
}