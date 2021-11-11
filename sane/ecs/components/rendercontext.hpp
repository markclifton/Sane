#pragma once

#include <glad/gl.h>

namespace Sane
{
    namespace Components
    {
        struct RenderContext
        {
            RenderContext(int32_t width, int32_t height)
                : width(width)
                , height(height)
            {
                glGenFramebuffers(1, &framebuffer);
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

                glGenTextures(2, attachments);

                glBindTexture(GL_TEXTURE_2D, attachments[0]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glGenerateMipmap(GL_TEXTURE_2D);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, attachments[0], 0);

                glBindTexture(GL_TEXTURE_2D, attachments[1]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glGenerateMipmap(GL_TEXTURE_2D);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachments[1], 0);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                valid = true;
            }

            bool valid{ 0 };
            uint32_t width{ 0 }, height{ 0 };
            uint32_t framebuffer{ 0 };
            uint32_t attachments[2]{ 0,0 };
        };
    }
}