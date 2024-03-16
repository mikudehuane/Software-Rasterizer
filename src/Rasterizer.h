#pragma once

#include "Buffer.h"
#include "Model.h"

namespace Islander
{

class Rasterizer
{
public:
	Rasterizer(const int width, const int height)
		: m_DepthBuffer(width, height)
		, m_ColorBuffer(width, height)
	{}

	void Render(const Model& model);

	const DepthBuffer& GetDepthBuffer() const { return m_DepthBuffer; }
	const ColorBuffer& GetColorBuffer() const { return m_ColorBuffer; }
private:
	[[nodiscard]] float GetY(const float y) const
	{
		// -1 ~ 1 -> 0 ~ height
		return (y + 1) * 0.5f * static_cast<float>(m_DepthBuffer.Height());
	}

	[[nodiscard]] float GetX(const float x) const
	{
		// -1 ~ 1 -> 0 ~ width
		return (x + 1) * 0.5f * static_cast<float>(m_ColorBuffer.Width());
	}

	// -1 ~ 1 -> screen coordinate
	void TransformToScreen(Triangle& triangle) const;

	void Rasterize(const Triangle& triangle);

	DepthBuffer m_DepthBuffer;
	ColorBuffer m_ColorBuffer;
};

}

