#include "Buffer.h"

namespace Islander
{

void BufferLayout::CalculateOffsetAndStride()
{
	m_Stride = 0;
	for (const BufferElement& element : m_Elements)
	{
		m_Stride += element.size * GetSize(element.type);
	}
}

VertexBuffer::VertexBuffer(BufferLayout layout, uint32_t size)
	: m_Data(static_cast<size_t>(layout.GetStride() * size))
	, m_Layout(std::move(layout))
{
}

}

