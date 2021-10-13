#include "sane/utils/aabb.hpp"

namespace Sane
{
    AABB::AABB()
        : minX(0), minY(0), minZ(0)
        , maxX(0), maxY(0), maxZ(0)
    {
    }

    AABB::AABB(const Vec3& min, const Vec3& max)
        : minX(min.x), minY(min.y), minZ(min.z)
        , maxX(max.x), maxY(max.y), maxZ(max.z)
    {}

    AABB::AABB(const Vec3* points, const std::size_t count)
        : minX(NAN), minY(NAN), minZ(NAN)
        , maxX(NAN), maxY(NAN), maxZ(NAN)
    {
        if (!count)
            return;
        minX = points[0].x;
        minY = points[0].y;
        minZ = points[0].z;
        maxX = points[0].x;
        maxY = points[0].y;
        maxZ = points[0].z;

        for (std::size_t i = 1u; i < count; i++) {
            if (points[i].x < minX)
                minX = points[i].x;
            else if (points[i].x > maxX)
                maxX = points[i].x;

            if (points[i].y < minY)
                minY = points[i].y;
            else if (points[i].y > maxY)
                maxY = points[i].y;

            if (points[i].z < minZ)
                minZ = points[i].z;
            else if (points[i].z > maxZ)
                maxZ = points[i].z;
        }
    }

    bool AABB::contains(const Vec3& point) const
    {
        return point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY && point.z >= minZ && point.z <= maxZ;
    }

    bool AABB::intersect(Vec3* isectPoint, const Ray& ray) const
    {
        const std::size_t signDirX = ray.direction.x < 0 ? 1u : 0u;
        const std::size_t signDirY = ray.direction.y < 0 ? 1u : 0u;
        const std::size_t signDirZ = ray.direction.z < 0 ? 1u : 0u;

        const Vec3 minPt(minX, minY, minZ);
        const Vec3 maxPt(maxX, maxY, maxZ);

        const Vec3 pts[2u]{ minPt, maxPt };

        float tmin = (pts[signDirX].x - ray.origin.x) / ray.direction.x;
        float tmax = (pts[1u - signDirX].x - ray.origin.x) / ray.direction.x;
        float tymin = (pts[signDirY].y - ray.origin.y) / ray.direction.y;
        float tymax = (pts[1u - signDirY].y - ray.origin.y) / ray.direction.y;

        if ((tmin > tymax) || (tymin > tymax))
            return false;

        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (pts[signDirZ].z - ray.origin.z) / ray.direction.z;
        float tzmax = (pts[1u - signDirZ].z - ray.origin.z) / ray.direction.z;

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        isectPoint->x = ray.origin.x + (ray.direction.x * tmin);
        isectPoint->y = ray.origin.y + (ray.direction.y * tmin);
        isectPoint->z = ray.origin.z + (ray.direction.z * tmin);

        return true;
    }

}