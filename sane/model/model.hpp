#pragma once

#include <vector>

#include "sane/graphics/common.hpp"
#include "sane/events/common.hpp"
#include "sane/utils/timer.hpp"

namespace Sane
{
    struct Vec3Packed;

    class Model : public Events::Listener
    {
    public:
        Model(const char* path);
        virtual bool ProcessEvent(Event& evt) override;
        void DrawImmediate();

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

        Vec3Packed* verticesPacked{ nullptr };
        unsigned int numVerticesPacked{ 0 };

        float x{ 0 }, y{ 0 };

        Timer timer;
        Timer timer2;
        double simdTotal{ 0 };
        int simdSamples{ 0 };

        double linearTotal{ 0 };
        int linearSamples{ 0 };
    };
}