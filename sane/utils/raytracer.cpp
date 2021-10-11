#include "sane/utils/raytracer.hpp"

namespace Sane
{
#include <cmath>
#include <limits>
#include <xmmintrin.h>
#include <smmintrin.h>

#include "sane/graphics/common.hpp"
#include "sane/logging/log.hpp"

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
        if (_mm_movemask_ps(detMask) == 0xff)
        {
            return false;
        }

        maskValid = _mm_andnot_ps(detMask, ones);

        __m128 invDet = _mm_div_ps(ones, det);

        Vec3Packed tvec = rayPack.origin - v0t;
        uPack = _mm_mul_ps(tvec.dotProduct(pvec), invDet);

        __m128 maskA = _mm_cmplt_ps(uPack, zeros);
        __m128 maskB = _mm_cmpgt_ps(uPack, ones);
        __m128 maskAB = _mm_or_ps(maskA, maskB);
        if (_mm_movemask_ps(maskAB) == 0xff)
        {
            return false;
        }

        maskValid = _mm_andnot_ps(maskAB, maskValid);

        Vec3Packed qvec = tvec.crossProduct(v0v1);
        vPack = _mm_mul_ps(rayPack.direction.dotProduct(qvec), invDet);

        maskA = _mm_cmplt_ps(vPack, zeros);
        maskB = _mm_cmpgt_ps(_mm_add_ps(uPack, vPack), ones);
        maskAB = _mm_or_ps(maskA, maskB);
        if (_mm_movemask_ps(maskAB) == 0xff)
        {
            return false;
        }

        maskValid = _mm_andnot_ps(maskAB, maskValid);

        tPack = _mm_mul_ps(v0v2.dotProduct(qvec), invDet);

        maskA = _mm_cmplt_ps(tPack, zerosE);
        maskValid = _mm_andnot_ps(maskA, maskValid);

        return _mm_movemask_ps(_mm_cmpeq_ps(maskValid, ones)) == 0x00 ? false : true;
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
}