#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

#include "Math.h"

namespace Islander
{

class DepthBuffer
{
public:
	DepthBuffer(const int width, const int height)
		: m_Width(width), m_Height(height)
		, m_Data(static_cast<size_t>(width * height), -1.0f)
	{
	}

	// buffer[{i, j}] to access row i col j
	const float& operator[](const std::pair<int, int> index) const
	{
		assert(index.first >= 0 && index.first < m_Height);
		assert(index.second >= 0 && index.second < m_Width);
		return m_Data[index.first * m_Width + index.second];
	}

	// buffer[{i, j}] to access row i col j
	float& operator[](const std::pair<int, int> index)
	{
		assert(index.first >= 0 && index.first < m_Height);
		assert(index.second >= 0 && index.second < m_Width);
		return m_Data[index.first * m_Width + index.second];
	}

	[[nodiscard]] int Width() const { return m_Width; }
	[[nodiscard]] int Height() const { return m_Height; }

	void DumpBmp(const std::string& path) const;
private:
	int m_Width, m_Height;
	std::vector<float> m_Data;
};

struct Color
{
	uint8_t r = 0, g = 0, b = 0, a = 255;
	explicit Color(const Vec3& color);
	Color() = default;
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		: r(r), g(g), b(b), a(a)
	{
	}
};

// pass color by value since its only 4 byte
inline Color Lerp(const Color a, const Color b, const float t)
{
	return {
		static_cast<uint8_t>(static_cast<float>(a.r) * (1 - t) + static_cast<float>(b.r) * t),
		static_cast<uint8_t>(static_cast<float>(a.g) * (1 - t) + static_cast<float>(b.g) * t),
		static_cast<uint8_t>(static_cast<float>(a.b) * (1 - t) + static_cast<float>(b.b) * t),
		static_cast<uint8_t>(static_cast<float>(a.a) * (1 - t) + static_cast<float>(b.a) * t)
	};
}

class ColorBuffer
{
public:
	ColorBuffer(const int width, const int height)
		: m_Width(width), m_Height(height)
		, m_Data(static_cast<size_t>(width * height))
	{
	}

	// buffer[{i, j}] to access row i col j
	const Color& operator[](const std::pair<int, int> index) const
	{
		assert(index.first >= 0 && index.first < m_Height);
		assert(index.second >= 0 && index.second < m_Width);
		return m_Data[index.first * m_Width + index.second];
	}

	// buffer[{i, j}] to access row i col j
	Color& operator[](const std::pair<int, int> index)
	{
		assert(index.first >= 0 && index.first < m_Height);
		assert(index.second >= 0 && index.second < m_Width);
		return m_Data[index.first * m_Width + index.second];
	}

	// Sample bilinearly
	[[nodiscard]]
	Color Sample(const Vec2& uv) const;
	[[nodiscard]]
	Vec3 SampleVec3(const Vec2& uv) const;

	[[nodiscard]] int Width() const { return m_Width; }
	[[nodiscard]] int Height() const { return m_Height; }

	void DumpBmp(const std::string& path) const;
private:
	int m_Width, m_Height;
	std::vector<Color> m_Data;
};

}
