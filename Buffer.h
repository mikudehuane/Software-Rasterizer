#pragma once

#include <cassert>
#include <string>
#include <vector>

namespace Islander
{

enum class DataType
{
	Float3, Float4
};

inline uint32_t GetSize(const DataType type)
{
	switch (type)
	{
	case DataType::Float3: return 4 * 3;
	case DataType::Float4: return 4 * 4;
	}

	assert(false);
	return 0;
}

// a property of a vertex (e.g., position)
struct BufferElement
{
	std::string name;
	DataType type;  // type of one data unit
	uint32_t size;  // number of data unit
};

class BufferLayout
{
public:
	BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}
	~BufferLayout() = default;

	// traverse elements
	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	[[nodiscard]]
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	[[nodiscard]]
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	// size of one group of element (e.g., a vertex) in bytes
	[[nodiscard]]
	uint32_t GetStride() const { return m_Stride; }

	BufferLayout(const BufferLayout&) = default;
	BufferLayout& operator=(const BufferLayout&) = default;
	BufferLayout(BufferLayout&&) = default;
	BufferLayout& operator=(BufferLayout&&) = default;
private:
	void CalculateOffsetAndStride();

	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride;
};

class VertexBuffer
{
public:
	/// @brief create a vertex buffer and its associated memory
	/// @param layout layout of one vertex
	/// @param size number of vertices
	VertexBuffer(BufferLayout layout, uint32_t size);
private:
	std::vector<std::byte> m_Data;
	BufferLayout m_Layout;
};

}

