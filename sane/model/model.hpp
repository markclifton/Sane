#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "sane/graphics/common.hpp"
#include "sane/events/common.hpp"
#include "sane/utils/timer.hpp"

namespace Sane
{
    class Model : public Events::Listener
    {
    public:
        Model(const char* path);
        virtual bool ProcessEvent(Event& evt) override;
        void DrawImmediate(glm::mat4 mvp);

        std::vector<VertexData::Position> vertices_;
        std::vector<VertexData::Normal> normals_;
        std::vector<VertexData::UV> uvs_;
        std::vector<uint32_t> indices_;

        bool buffered_{ false };

        ShaderProgram sProg_;
        Buffer vertices_buffer;
        Buffer normals_buffer;
        Buffer uvs_buffer;
        Buffer indices_buffer;
        VertexAttrib vPos;
    };
}