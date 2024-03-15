#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace Islander
{

class DepthBuffer
{
public:
	DepthBuffer(const int width, const int height)
		: m_Width(width), m_Height(height)
		, m_Data(static_cast<size_t>(width * height))
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

	void DumpBmp(const std::string& path) const;
private:
	int m_Width, m_Height;
	std::vector<float> m_Data;
};
	
}
