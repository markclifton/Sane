#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace Sane
{
    class Framebuffer
    {
    public:
        Framebuffer(int32_t width, int32_t height);
        ~Framebuffer();

        void Resize(int32_t width, int32_t height);
        void Invalidate();
        void Bind();
        void Unbind();
        void Clear();

        uint32_t GetAttachment(uint32_t index);
        const glm::vec2 GetSize();

    private:
        glm::vec2 size_{ 0, 0 };
        uint32_t framebuffer_{ 0 };
        uint32_t attachments_[2]{ 0,0 };
    };
}