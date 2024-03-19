#pragma once

#include <cmath>

namespace Islander
{

struct Vec3
{
	float x, y, z;
};

struct Vec2
{
	float x, y;
};

inline float Cross(const Vec2& opLeft, const Vec2& opRight)
{
	return opLeft.x * opRight.y - opLeft.y * opRight.x;
}

inline Vec2 operator-(const Vec2& opLeft, const Vec2& opRight)
{
	return { opLeft.x - opRight.x, opLeft.y - opRight.y };
}

inline Vec2 operator*(const float opLeft, const Vec2& opRight)
{
	return { opLeft * opRight.x, opLeft * opRight.y };
}

inline Vec2 operator+(const Vec2& opLeft, const Vec2& opRight)
{
	return { opLeft.x + opRight.x, opLeft.y + opRight.y };
}

inline Vec3 operator*(const float opLeft, const Vec3& opRight)
{
	return { opLeft * opRight.x, opLeft * opRight.y, opLeft * opRight.z };
}

inline Vec3 operator*(const Vec3& opLeft, const Vec3& opRight)
{
	return { opLeft.x * opRight.x, opLeft.y * opRight.y, opLeft.z * opRight.z };
}

inline Vec3 operator+(const Vec3& opLeft, const Vec3& opRight)
{
	return { opLeft.x + opRight.x, opLeft.y + opRight.y, opLeft.z + opRight.z };
}

// overload minus
inline Vec3 operator-(const Vec3& op)
{
	return { -op.x, -op.y, -op.z };
}

inline Vec3 operator-(const Vec3& opLeft, const Vec3& opRight)
{
	return { opLeft.x - opRight.x, opLeft.y - opRight.y, opLeft.z - opRight.z };
}

inline float Dot(const Vec3& opLeft, const Vec3& opRight)
{
	return opLeft.x * opRight.x + opLeft.y * opRight.y + opLeft.z * opRight.z;
}

// reflect normalized vec3
inline Vec3 Reflect(const Vec3& incident, const Vec3& normal)
{
	return incident + 2.0f * Dot(-incident, normal) * normal;
}

inline Vec3 Normalize(const Vec3& op)
{
	const float length = std::sqrt(Dot(op, op));
	return { op.x / length, op.y / length, op.z / length };
}

inline float Clamp(const float value, const float min, const float max)
{
	return value < min ? min : (value > max ? max : value);
}

inline int Clamp(const int value, const int min, const int max)
{
	return value < min ? min : (value > max ? max : value);
}

}

