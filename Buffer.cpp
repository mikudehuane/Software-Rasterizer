#include "Buffer.h"

namespace Islander
{

BufferLayout BufferLayout::Packed() const
{
	std::vector<BufferElement> newLayout;
	newLayout.reserve(m_Elements.size());
	size_t offset = 0;
	for (const BufferElement& element : m_Elements)
	{
		newLayout.push_back({
			.name = element.name,
			.type = element.type,
			.size = element.size,
			.offset = offset
		});
		offset += GetSize(element.type) * element.size;
	}
	BufferLayout ret;
	ret.m_Elements = std::move(newLayout);
	ret.m_Stride = offset;
	ret.Init();
	return ret;
}

const BufferElement& BufferLayout::GetElement(const std::string& name) const
{
	const size_t index = m_Name2Index.at(name);
	return m_Elements[index];
}

void BufferLayout::Init()
{
	for (size_t i = 0; i < m_Elements.size(); ++i)
	{
		const BufferElement& element = m_Elements[i];
		m_Name2Index[element.name] = i;
	}
}

}

