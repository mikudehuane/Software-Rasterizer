#pragma once

#include "Buffer.h"

#include <cstdint>
#include <string>
#include <vector>

namespace Islander
{

inline uint8_t ToU8Color(const float value)
{
	return static_cast<uint8_t>(
		std::max(0, std::min(255, static_cast<int>(value * 256.0f)))
	);
}

class DepthBuffer
{
public:
	DepthBuffer(const size_t width, const size_t height)
		: m_Width(width), m_Height(height)
		, m_Data(width * height)
	{
	}

	class Range
	{
	public:
		float& operator[](size_t index) const
		{
			assert(index < m_Width);
			return m_Start[index];
		}

		friend class DepthBuffer;
	private:
		Range(float* start, const size_t width)
			: m_Start(start), m_Width(width) {}
		float* m_Start;
		size_t m_Width;
	};

	// buffer[i][j] to access row i col j
	Range operator[](const size_t index)
	{
		assert(index < m_Height);
		return { m_Data.data() + index * m_Width, m_Width };
	}
	void DumpBmp(const std::string& path, const BufferElement& colorElement);
private:
	size_t m_Width, m_Height;
	std::vector<float> m_Data;
};
	
}
