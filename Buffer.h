#pragma once

#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace Islander
{

enum class DataType
{
	Float
};

inline size_t GetSize(const DataType type)
{
	switch (type)
	{
	case DataType::Float: return sizeof(float);
	}

	assert(false);
	return 0;
}

// a property of a vertex (e.g., position)
struct BufferElement
{
	std::string name;
	DataType type;  // type of one data unit
	size_t size;  // number of data unit
	size_t offset;  // offset of current element
};

class BufferLayout
{
public:
	BufferLayout(const std::initializer_list<BufferElement>& elements, const size_t stride)
		: m_Elements(elements), m_Stride(stride)
	{
		Init();
	}
	~BufferLayout() = default;

	// create a tightly packed layout from *this
	[[nodiscard]]
	BufferLayout Packed() const;

	// traverse elements
	[[nodiscard]]
	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	[[nodiscard]]
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	[[nodiscard]]
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	[[nodiscard]]
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	[[nodiscard]]
	const BufferElement& GetElement(const std::string& name) const;

	// size of one group of element (e.g., a vertex) in bytes
	[[nodiscard]]
	size_t GetStride() const { return m_Stride; }

	BufferLayout(const BufferLayout&) = default;
	BufferLayout& operator=(const BufferLayout&) = default;
	BufferLayout(BufferLayout&&) = default;
	BufferLayout& operator=(BufferLayout&&) = default;
private:
	BufferLayout() = default;
	void Init();

	std::vector<BufferElement> m_Elements;
	std::map<std::string, size_t> m_Name2Index;
	size_t m_Stride;
};

class VertexBuffer
{
public:
	/// @brief create a vertex buffer and its associated memory
	/// @param layout layout of one vertex
	/// @param size number of vertices
	VertexBuffer(BufferLayout layout, const size_t size)
		: m_Data(layout.GetStride()* size)
		, m_Layout(std::move(layout))
	{}
private:
	std::vector<std::byte> m_Data;
	BufferLayout m_Layout;
};

}

