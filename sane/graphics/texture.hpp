#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace Sane
{
    class Texture
    {
    public:
        Texture(const char* path);
        ~Texture();

        void Bind();
        void Unbind();

        GLuint GetTextureId();
        glm::vec2 GetSize();

    private:
        glm::vec2 size{ 0, 0 };
        GLuint texture{ 0 };
    };
}