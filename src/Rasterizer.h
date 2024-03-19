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

	[[nodiscard]] const DepthBuffer& GetDepthBuffer() const { return m_DepthBuffer; }
	[[nodiscard]] const ColorBuffer& GetColorBuffer() const { return m_ColorBuffer; }
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

	void Rasterize();
	void FragmentShading();

	DepthBuffer m_DepthBuffer;
	ColorBuffer m_ColorBuffer;

	// runtime states
	const Material* m_Material = nullptr;
	const Triangle* m_Triangle = nullptr;
	const Vec2* m_V0 = nullptr;
	const Vec2* m_V1 = nullptr;
	const Vec2* m_V2 = nullptr;
	const Vec2* m_VMid = nullptr;   // y is middle
	const Vec2* m_VTop = nullptr;   // y is highest
	Vec2 m_P = { 0.0f, 0.0f };  // rendering point
	float m_Area = 0.0f;  // triangle area
	int m_X = 0, m_Y = 0;  // screen coordinate
};

}

