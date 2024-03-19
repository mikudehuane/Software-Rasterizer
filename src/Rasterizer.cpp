#include "Rasterizer.h"

#include <iostream>

#include "Model.h"
#include "Shaders.h"

namespace Islander
{

void SortTriangle(Triangle& triangle)
{
	// v0 at bottom
	{
		if (triangle.v0.position.y > triangle.v1.position.y)
		{
			std::swap(triangle.v0, triangle.v1);
		}
		if (triangle.v0.position.y > triangle.v2.position.y)
		{
			std::swap(triangle.v0, triangle.v2);
		}
	}

	// arrange v0-v1-v2 to be counter-clockwise
	if (Cross(
		{ triangle.v1.position.x - triangle.v0.position.x, triangle.v1.position.y - triangle.v0.position.y },
		{ triangle.v2.position.x - triangle.v0.position.x, triangle.v2.position.y - triangle.v0.position.y }
	) < 0)
	{
		std::swap(triangle.v1, triangle.v2);
	}
}

void Rasterizer::Rasterize()
{
	constexpr float eps = 1e-7f;

	m_V0 = reinterpret_cast<const Vec2*>(&m_Triangle->v0.position);
	m_V1 = reinterpret_cast<const Vec2*>(&m_Triangle->v1.position);
	m_V2 = reinterpret_cast<const Vec2*>(&m_Triangle->v2.position);
	m_VMid = m_V1->y < m_V2->y ? m_V1 : m_V2;
	m_VTop = m_V1->y < m_V2->y ? m_V2 : m_V1;

	const float yMin = std::max(m_V0->y, 0.0f);
	const float yMax = std::min(m_VTop->y, static_cast<float>(m_DepthBuffer.Height() - 1));
	const float yMid = std::max(yMin, std::min(yMax, m_VMid->y));

	// v01 x v02
	m_Area = Cross(*m_V1 - *m_V2, *m_V2 - *m_V0);

	// lower half
	{
		float tLeft = (m_V2->x - m_V0->x) / (m_V2->y - m_V0->y + eps);
		float tRight = (m_V1->x - m_V0->x) / (m_V1->y - m_V0->y + eps);
		if (tLeft > tRight)
		{
			std::swap(tLeft, tRight);
		}
		for (m_Y = static_cast<int>(yMin); m_Y < static_cast<int>(yMid); ++m_Y)
		{
			const float yCoord = static_cast<float>(m_Y) + 0.5f;
			const float yDiff = yCoord - m_V0->y;
			float xMin = tLeft * yDiff + m_V0->x;
			float xMax = tRight * yDiff + m_V0->x;
			xMin = std::max(0.0f, xMin);
			xMax = std::min(static_cast<float>(m_DepthBuffer.Width() - 1), xMax);

			for (m_X = static_cast<int>(xMin); static_cast<float>(m_X) <= xMax; ++m_X)
			{
				// pixel (x, y): coordinate (x + 0.5, y + 0.5)
				m_P = { static_cast<float>(m_X) + 0.5f, yCoord };
				FragmentShading();
			}
		}
	}

	// higher half
	{
		float tLeft = (m_VMid->x - m_VTop->x) / (m_VMid->y - m_VTop->y + eps);
		float tRight = (m_V0->x - m_VTop->x) / (m_V0->y - m_VTop->y + eps);
		if (tLeft < tRight)
		{
			std::swap(tLeft, tRight);
		}

		for (m_Y = static_cast<int>(yMid); m_Y <= static_cast<int>(yMax); ++m_Y)
		{
			const float yCoord = static_cast<float>(m_Y) + 0.5f;
			const float yDiff = yCoord - m_VTop->y;
			float xMin = tLeft * yDiff + m_VTop->x;
			float xMax = tRight * yDiff + m_VTop->x;
			xMin = std::max(0.0f, xMin);
			xMax = std::min(static_cast<float>(m_DepthBuffer.Width() - 1), xMax);

			for (m_X = static_cast<int>(xMin); m_X <= static_cast<int>(xMax); ++m_X)
			{
				// pixel (x, y): coordinate (x + 0.5, y + 0.5)
				m_P = { static_cast<float>(m_X) + 0.5f, yCoord };
				FragmentShading();
			}
		}
	}

}

void Rasterizer::FragmentShading()
{
	// bi-centric coordinates of (xCoord, yCoord) in triangle
	// p-1 x p-2
	const float w0 = Cross(*m_V1 - m_P, *m_V2 - m_P) / m_Area;
	// 0-p x 0-2
	const float w1 = Cross(m_P - *m_V0, *m_V2 - *m_V0) / m_Area;
	// ReSharper disable once CppTooWideScopeInitStatement
	const float w2 = 1 - w0 - w1;

	// fragment shade only inside triangle
	if (w0 >= 0 && w1 >= 0 && w2 >= 0)
	{
		const float z = w0 * m_Triangle->v0.position.z
			+ w1 * m_Triangle->v1.position.z
			+ w2 * m_Triangle->v2.position.z;
		float& element = m_DepthBuffer[{m_Y, m_X}];
		if (z > element)  // depth checking
		{
			element = z;
			const Vec2 texCoord = w0 * m_Triangle->v0.texCoord
				+ w1 * m_Triangle->v1.texCoord
				+ w2 * m_Triangle->v2.texCoord;
			const Vec3 normal = w0 * m_Triangle->v0.normal
				+ w1 * m_Triangle->v1.normal
				+ w2 * m_Triangle->v2.normal;
			const Vec3 shaded = PhongShading(*m_Material, texCoord, normal);
			m_ColorBuffer[{m_Y, m_X}] = static_cast<Color>(shaded);
		}
	}
}

void Rasterizer::Render(const Model& model)
{
	for (const Mesh& mesh : model.meshes)
	{
		m_Material = mesh.material.get();
		for (const Triangle& triangle : mesh.triangles)
		{
			Triangle transformed = triangle;
			SortTriangle(transformed);
			TransformToScreen(transformed);
			m_Triangle = &transformed;
			Rasterize();
		}
	}
}

void Rasterizer::TransformToScreen(Triangle& triangle) const
{
	triangle.v0.position.x = GetX(triangle.v0.position.x);
	triangle.v0.position.y = GetY(triangle.v0.position.y);
	triangle.v1.position.x = GetX(triangle.v1.position.x);
	triangle.v1.position.y = GetY(triangle.v1.position.y);
	triangle.v2.position.x = GetX(triangle.v2.position.x);
	triangle.v2.position.y = GetY(triangle.v2.position.y);
}

}

