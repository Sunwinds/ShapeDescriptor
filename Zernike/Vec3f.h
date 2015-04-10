//
// Vec3f.h
//

#ifndef __VEC3F_H
#define __VEC3F_H

#include <math.h>

struct Vec3f
{
    //
    // Initalization
    //
    Vec3f()
    {

    }
    Vec3f(const Vec3f &V)
    {
        x = V.x;
        y = V.y;
        z = V.z;
    }
    Vec3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    //
    // Overloaded operators
    //
    Vec3f& operator *= (float Right)
    {
        x *= Right;
        y *= Right;
        z *= Right;
        return *this;
    }
    Vec3f& operator /= (float Right)
    {
        x /= Right;
        y /= Right;
        z /= Right;
        return *this;
    }
    Vec3f& operator += (const Vec3f &Right)
    {
        x += Right.x;
        y += Right.y;
        z += Right.z;
        return *this;
    }
    Vec3f& operator -= (const Vec3f &Right)
    {
        x -= Right.x;
        y -= Right.y;
        z -= Right.z;
        return *this;
    }

    //
    // Accessors
    //
    float Length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }
    float LengthSq() const
    {
        return x * x + y * y + z * z;
    }

    //
    // Local data
    //
    float x, y, z;

    //
    // Static functions
    //
    static Vec3f Normalize(const Vec3f &V)
    {
        float Len = V.Length();
        if(Len == 0.0f)
        {
            return V;
        }
        else
        {
            float Factor = 1.0f / Len;
            return Vec3f(V.x * Factor, V.y * Factor, V.z * Factor);
        }
    }
    static Vec3f Cross(const Vec3f &Left, const Vec3f &Right)
    {
        Vec3f Result;
        Result.x = Left.y * Right.z - Left.z * Right.y;
        Result.y = Left.z * Right.x - Left.x * Right.z;
        Result.z = Left.x * Right.y - Left.y * Right.x;
        return Result;
    }
    static float Dot(const Vec3f &Left, const Vec3f &Right)
    {
        return (Left.x * Right.x + Left.y * Right.y + Left.z * Right.z);
    }
    static float Dist(const Vec3f &Left, const Vec3f &Right)
    {
        const float XDiff = Right.x - Left.x;
        const float YDiff = Right.y - Left.y;
        const float ZDiff = Right.z - Left.z;
        return sqrtf(XDiff * XDiff + YDiff * YDiff + ZDiff * ZDiff);
    }
    static float DistSq(const Vec3f &Left, const Vec3f &Right)
    {
        const float XDiff = Right.x - Left.x;
        const float YDiff = Right.y - Left.y;
        const float ZDiff = Right.z - Left.z;
        return (XDiff * XDiff + YDiff * YDiff + ZDiff * ZDiff);
    }

    static Vec3f Maximize(const Vec3f &Left, const Vec3f &Right)
    {
        Vec3f Result = Right;
        if(Left.x > Right.x) Result.x = Left.x;
        if(Left.y > Right.y) Result.y = Left.y;
        if(Left.z > Right.z) Result.z = Left.z;
        return Result;
    }

    static Vec3f Minimize(const Vec3f &Left, const Vec3f &Right)
    {
        Vec3f Result = Right;
        if(Left.x < Right.x) Result.x = Left.x;
        if(Left.y < Right.y) Result.y = Left.y;
        if(Left.z < Right.z) Result.z = Left.z;
        return Result;
    }
};

inline Vec3f operator * (const Vec3f &Left, float Right)
{
    return Vec3f(Left.x * Right,
        Left.y * Right,
        Left.z * Right);
}

inline Vec3f operator * (float Left, const Vec3f &Right)
{
    return Vec3f(Right.x * Left,
        Right.y * Left,
        Right.z * Left);
}

inline Vec3f operator / (const Vec3f &Left, float Right)
{
    return Vec3f(Left.x / Right,
        Left.y / Right,
        Left.z / Right);
}

inline Vec3f operator + (const Vec3f &Left, const Vec3f &Right)
{
    return Vec3f(Left.x + Right.x,
        Left.y + Right.y,
        Left.z + Right.z);
}

inline Vec3f operator - (const Vec3f &Left, const Vec3f &Right)
{
    return Vec3f(Left.x - Right.x,
        Left.y - Right.y,
        Left.z - Right.z);
}

inline Vec3f operator - (const Vec3f &V)
{
    return Vec3f(-V.x, -V.y, -V.z);
}

#endif