#include "sane/utils/raytracer.hpp"

#include <cmath>
#include <limits>
#include <xmmintrin.h>
#include <smmintrin.h>

#include "sane/graphics/common.hpp"
#include "sane/logging/log.hpp"
#include "sane/model/model.hpp"
#include "sane/utils/aabb.hpp"

namespace Sane
{
    Vec3Packed::Vec3Packed() {}
    Vec3Packed::~Vec3Packed() {}

    Vec3Packed::Vec3Packed(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3)
        : Vec3Packed(_mm_set_ps(v3.x, v2.x, v1.x, v0.x), _mm_set_ps(v3.y, v2.y, v1.y, v0.y), _mm_set_ps(v3.z, v2.z, v1.z, v0.z))
    {
    }
    Vec3Packed::Vec3Packed(__m128 x, __m128 y, __m128 z)
        : packedX(x), packedY(y), packedZ(z)
    {}

    float Vec3Packed::get0(__m128& pack) const
    {
        return _mm_cvtss_f32(pack);
    }

    float Vec3Packed::get1(__m128& pack) const
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(pack, pack, _MM_SHUFFLE(1, 1, 1, 1)));
    }

    float Vec3Packed::get2(__m128& pack) const
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(pack, pack, _MM_SHUFFLE(2, 2, 2, 2)));
    }

    float Vec3Packed::get3(__m128& pack) const
    {
        return _mm_cvtss_f32(_mm_shuffle_ps(pack, pack, _MM_SHUFFLE(3, 3, 3, 3)));
    }

    __m128 Vec3Packed::dotProduct(const Vec3Packed& pack1) const
    {
        return
            _mm_add_ps(
                _mm_mul_ps(packedX, pack1.packedX),
                _mm_add_ps(
                    _mm_mul_ps(packedY, pack1.packedY),
                    _mm_mul_ps(packedZ, pack1.packedZ)
                )
            );
    }

    Vec3Packed Vec3Packed::crossProduct(const Vec3Packed& v1) const
    {
        return {
            _mm_sub_ps(_mm_mul_ps(packedY, v1.packedZ), _mm_mul_ps(packedZ, v1.packedY)),
            _mm_sub_ps(_mm_mul_ps(packedZ, v1.packedX), _mm_mul_ps(packedX, v1.packedZ)),
            _mm_sub_ps(_mm_mul_ps(packedX, v1.packedY), _mm_mul_ps(packedY, v1.packedX))
        };
    }


    Vec3Packed operator* (const Vec3Packed& v0, const Vec3Packed& v1)
    {
        return {
            _mm_mul_ps(v0.packedX, v1.packedX),
            _mm_mul_ps(v0.packedY, v1.packedY),
            _mm_mul_ps(v0.packedZ, v1.packedZ)
        };
    }

    Vec3Packed operator+ (const Vec3Packed& v0, const Vec3Packed& v1)
    {
        return {
            _mm_add_ps(v0.packedX, v1.packedX),
            _mm_add_ps(v0.packedY, v1.packedY),
            _mm_add_ps(v0.packedZ, v1.packedZ)
        };
    }

    Vec3Packed operator- (const Vec3Packed& v0, const Vec3Packed& v1)
    {
        return {
            _mm_sub_ps(v0.packedX, v1.packedX),
            _mm_sub_ps(v0.packedY, v1.packedY),
            _mm_sub_ps(v0.packedZ, v1.packedZ)
        };
    }

    RayPacked::RayPacked() {}
    RayPacked::~RayPacked() {}
    RayPacked::RayPacked(const Ray& ray)
        : origin({ ray.origin, ray.origin, ray.origin, ray.origin })
        , direction({ ray.direction, ray.direction, ray.direction, ray.direction })
    {}

    bool intersectTrianglesPacked(const RayPacked& rayPack, const Vec3Packed& vPack0, const Vec3Packed& vPack1, const Vec3Packed& vPack2, __m128& tPack, __m128& maskValid, __m128& uPack, __m128& vPack)
    {
        static const __m128 zeros = _mm_setzero_ps();
        static const __m128 zerosE = _mm_set1_ps(0.000001f);
        static const __m128 ones = _mm_set1_ps(1.0f);
        static const __m128 maskFloatSign = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

        Vec3Packed v0t = vPack0;
        Vec3Packed v1t = vPack1;
        Vec3Packed v2t = vPack2;

        Vec3Packed v0v1 = v1t - v0t;
        Vec3Packed v0v2 = v2t - v0t;
        Vec3Packed pvec = rayPack.direction.crossProduct(v0v2);
        __m128 det = v0v1.dotProduct(pvec);

        __m128 detMask = _mm_cmplt_ps(_mm_andnot_ps(maskFloatSign, det), zerosE);
        if (_mm_movemask_ps(detMask) == 0xff) return false;

        maskValid = _mm_andnot_ps(detMask, ones);

        __m128 invDet = _mm_div_ps(ones, det);

        Vec3Packed tvec = rayPack.origin - v0t;
        uPack = _mm_mul_ps(tvec.dotProduct(pvec), invDet);

        __m128 maskA = _mm_cmplt_ps(uPack, zeros);
        __m128 maskB = _mm_cmpgt_ps(uPack, ones);
        __m128 maskAB = _mm_or_ps(maskA, maskB);
        if (_mm_movemask_ps(maskAB) == 0xff) return false;

        maskValid = _mm_andnot_ps(maskAB, maskValid);

        Vec3Packed qvec = tvec.crossProduct(v0v1);
        vPack = _mm_mul_ps(rayPack.direction.dotProduct(qvec), invDet);

        maskA = _mm_cmplt_ps(vPack, zeros);
        maskB = _mm_cmpgt_ps(_mm_add_ps(uPack, vPack), ones);
        maskAB = _mm_or_ps(maskA, maskB);
        if (_mm_movemask_ps(maskAB) == 0xff) return false;

        maskValid = _mm_andnot_ps(maskAB, maskValid);

        tPack = _mm_mul_ps(v0v2.dotProduct(qvec), invDet);

        maskA = _mm_cmplt_ps(tPack, zerosE);
        maskValid = _mm_andnot_ps(maskA, maskValid);

        return _mm_movemask_ps(_mm_cmpeq_ps(maskValid, ones)) != 0x00;
    }

    bool intersect(Vec3Packed* VerticesPacked, unsigned int NumberOfVertexPacks, const Ray& ray, float& distance, unsigned int& triangleIndex, float& u, float& v)
    {
        bool isect = false;
        RayPacked rayPack(ray);

        unsigned int triangleIndexPacked = 0;
        for (unsigned int i = 0; i < NumberOfVertexPacks; i += 3)
        {
            const Vec3Packed& pack0 = VerticesPacked[i];
            const Vec3Packed& pack1 = VerticesPacked[i + 1];
            const Vec3Packed& pack2 = VerticesPacked[i + 2];

            static float max = std::numeric_limits<float>::max();

            __m128 packT = _mm_set1_ps(max);
            __m128 mask;
            __m128 packU;
            __m128 packV;

            if (intersectTrianglesPacked(rayPack, pack0, pack1, pack2, packT, mask, packU, packV))
            {
                float tArray[4];
                _mm_store_ps(tArray, packT);

                float maskArray[4];
                _mm_store_ps(maskArray, mask);

                float uArray[4];
                _mm_store_ps(uArray, packU);

                float vArray[4];
                _mm_store_ps(vArray, packV);

                for (int j = 0; j < 4; j++)
                {
                    if (tArray[j] > 0 && tArray[j] < distance && maskArray[j])
                    {
                        distance = tArray[j];
                        triangleIndex = triangleIndexPacked + j;
                        u = uArray[j];
                        v = vArray[j];
                        isect = true;
                    }
                }
            }
            triangleIndexPacked += 4;
        }
        return isect;
    }

    float dot(const Vec3& left, const Vec3& right)
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    Vec3 cross(const Vec3& left, const Vec3& right)
    {
        Vec3 result;
        result.x = left.y * right.z - left.z * right.y;
        result.y = left.z * right.x - left.x * right.z;
        result.z = left.x * right.y - left.y * right.x;
        return result;
    }

    int Triangle_intersect(Vec3* value,
        const double V0x, const double V0y, const double V0z,
        const double V1x, const double V1y, const double V1z,
        const double V2x, const double V2y, const double V2z,
        const Ray& ray)
    {
        Vec3 u(0.0, 0.0, 0.0), v(0.0, 0.0, 0.0), n(0.0, 0.0, 0.0);
        Vec3 dir(0.0, 0.0, 0.0), w0(0.0, 0.0, 0.0), w(0.0, 0.0, 0.0);
        double r, a, b;

        u = Vec3(V1x - V0x, V1y - V0y, V1z - V0z);
        v = Vec3(V2x - V0x, V2y - V0y, V2z - V0z);
        n = cross(u, v);

        if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0)
            return 1;

        dir = ray.direction;

        w0 = Vec3(ray.origin.x - V0x, ray.origin.y - V0y, ray.origin.z - V0z);
        a = -dot(n, w0);

        b = dot(n, dir);
        if (std::abs(b) < 1e-13)
            return 1;

        r = a / b;
        if (r < 0.0)
            return 1;

        Vec3 I(ray.origin.x + (dir.x * r), ray.origin.y + (dir.y * r), ray.origin.z + (dir.z * r));

        double    uu, uv, vv, wu, wv, D;
        uu = dot(u, u);
        uv = dot(u, v);
        vv = dot(v, v);

        w = Vec3(I.x - V0x, I.y - V0y, I.z - V0z);
        wu = dot(w, u);
        wv = dot(w, v);
        D = uv * uv - uu * vv;

        double s, t;
        s = (uv * wv - vv * wu) / D;
        if (s < 0.0 || s > 1.0)
            return 1;
        t = (uv * wu - uu * wv) / D;
        if (t < 0.0 || (s + t) > 1.0)
            return 1;

        value->x = I.x;
        value->y = I.y;
        value->z = I.z;
        return 0;
    }

    bool intersect2(Model* model, Vec3* value, const Ray& ray)
    {
        const AABB aabb_wcs(&model->vertices_[0], model->vertices_.size());
        const bool aabb_isect = aabb_wcs.intersect(value, ray);
        const bool aabb_contains = (ray.origin.x >= aabb_wcs.minX && ray.origin.x <= aabb_wcs.maxX &&
            ray.origin.y >= aabb_wcs.minY && ray.origin.y <= aabb_wcs.maxY &&
            ray.origin.z >= aabb_wcs.minZ && ray.origin.z <= aabb_wcs.maxZ);

        // screen for AABB intersection
        if (!aabb_isect && !aabb_contains)
            return false;

        return intersectGeneric2(model, value, ray);
    }

    bool intersectGeneric2(Model* model, Vec3* value, const Ray& ray)
    {
        std::size_t s = 3u;

        double isectDistSq = NAN;
        bool haveCandidate = false;

        const std::size_t faceCount = model->indices_.size() / 3;
        for (std::size_t face = 0; face < faceCount; face++) {
            Vec3 a = model->vertices_[face * 3 + 0];
            Vec3 b = model->vertices_[face * 3 + 1];
            Vec3 c = model->vertices_[face * 3 + 2];

            Vec3 abc[3] = { a, b, c };

            AABB aabb(abc, 3u);
            Vec3 p;
            if (!aabb.intersect(&p, ray))
                continue;

            if (haveCandidate) {
                const double dx = (p.x - ray.origin.x);
                const double dy = (p.y - ray.origin.y);
                const double dz = (p.z - ray.origin.z);
                const double distSq = (dx * dx) + (dy * dy) + (dz * dz);
                if (distSq > isectDistSq)
                    continue;
            }

            int code = Triangle_intersect(&p,
                a.x, a.y, a.z,
                b.x, b.y, b.z,
                c.x, c.y, c.z,
                ray);
            if (code != 0)
                continue;

            const double dx = (p.x - ray.origin.x);
            const double dy = (p.y - ray.origin.y);
            const double dz = (p.z - ray.origin.z);
            const double distSq = (dx * dx) + (dy * dy) + (dz * dz);
            if (!haveCandidate || (distSq < isectDistSq)) {
                *value = p;
                isectDistSq = distSq;
                haveCandidate = true;
            }
        }

        if (!haveCandidate)
            return false;

        return true;
    }


}