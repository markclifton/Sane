#pragma once

#include <glad/gl.h>

namespace Sane
{
    class Framebuffer
    {
    public:
        Framebuffer(size_t Width, size_t Height);
        ~Framebuffer();

        void Bind();
        void Unbind();
        void Clear();

        GLuint GetAttachment(size_t index);

    private:
        GLuint framebuffer;
        GLuint attachments[2];
    };
}