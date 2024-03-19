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

inline Vec2 operator*(float opLeft, const Vec2& opRight)
{
	return { opLeft * opRight.x, opLeft * opRight.y };
}

inline Vec2 operator+(const Vec2& opLeft, const Vec2& opRight)
{
	return { opLeft.x + opRight.x, opLeft.y + opRight.y };
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

