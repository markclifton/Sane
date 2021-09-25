#pragma once

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

        uint32_t GetTextureId();
        glm::vec2 GetSize();

    private:
        glm::vec2 size{ 0, 0 };
        uint32_t texture{ 0 };
    };
}