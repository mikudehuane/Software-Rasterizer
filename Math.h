#pragma once

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

}

