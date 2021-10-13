#pragma once

#include "sane/graphics/common.hpp"
#include "sane/utils/raytracer.hpp"

namespace Sane
{
    class AABB
    {
    public:
        AABB();
        AABB(const Vec3& min, const Vec3& max);
        AABB(const Vec3* points, const std::size_t numPoints);

        bool contains(const Vec3& point) const;
        bool intersect(Vec3* isectPoint, const Ray& ray) const;

        float minX, minY, minZ;
        float maxX, maxY, maxZ;
    };
}