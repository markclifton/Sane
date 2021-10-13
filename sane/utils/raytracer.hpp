#pragma once

#include <cmath>
#include <limits>
#include <xmmintrin.h>
#include <smmintrin.h>

#include "sane/graphics/common.hpp"
#include "sane/logging/log.hpp"

namespace Sane
{
    typedef VertexData::Position Vec3;

    struct Ray
    {
        Vec3 origin;
        Vec3 direction;
    };

    struct Vec3Packed
    {
        Vec3Packed();
        ~Vec3Packed();

        Vec3Packed(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3);
        Vec3Packed(__m128 x, __m128 y, __m128 z);

        float get0(__m128& pack) const;
        float get1(__m128& pack) const;
        float get2(__m128& pack) const;
        float get3(__m128& pack) const;

        __m128 dotProduct(const Vec3Packed& pack1) const;
        Vec3Packed crossProduct(const Vec3Packed& v1) const;

        friend Vec3Packed operator * (const Vec3Packed& v0, const Vec3Packed& v1);
        friend Vec3Packed operator + (const Vec3Packed& v0, const Vec3Packed& v1);
        friend Vec3Packed operator - (const Vec3Packed& v0, const Vec3Packed& v1);

        __m128 packedX;
        __m128 packedY;
        __m128 packedZ;
    };

    Vec3Packed operator* (const Vec3Packed& v0, const Vec3Packed& v1);
    Vec3Packed operator+ (const Vec3Packed& v0, const Vec3Packed& v1);
    Vec3Packed operator- (const Vec3Packed& v0, const Vec3Packed& v1);

    struct RayPacked
    {
        RayPacked();
        ~RayPacked();
        RayPacked(const Ray& ray);

        Vec3Packed origin;
        Vec3Packed direction;
    };

    bool intersectTrianglesPacked(const Vec3Packed& scalePack, const Vec3Packed& positionPack, const RayPacked& rayPack, const Vec3Packed& vPack0, const Vec3Packed& vPack1, const Vec3Packed& vPack2, __m128& tPack, __m128& maskValid, __m128& uPack, __m128& vPack);
    bool intersect(Vec3Packed* VerticesPacked, unsigned int NumberOfVertexPacks, const Ray& ray, float& distance, unsigned int& triangleIndex, float& u, float& v);

    float dot(const Vec3& left, const Vec3& right);
    Vec3 cross(const Vec3& left, const Vec3& right);
    int Triangle_intersect(Vec3* value,
        const double V0x, const double V0y, const double V0z,
        const double V1x, const double V1y, const double V1z,
        const double V2x, const double V2y, const double V2z,
        const Ray& ray);

    class Model;
    bool intersect2(Model* model, Vec3* value, const Ray& ray);
    bool intersectGeneric2(Model* model, Vec3* value, const Ray& ray);
}