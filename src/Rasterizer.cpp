#include "Rasterizer.h"

#include <iostream>

#include "Model.h"

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

void Rasterizer::Rasterize(const Triangle& triangle)
{
	constexpr float eps = 1e-7f;

	const Vec2& v0 = *reinterpret_cast<const Vec2*>(&triangle.v0);
	const Vec2& v1 = *reinterpret_cast<const Vec2*>(&triangle.v1);
	const Vec2& v2 = *reinterpret_cast<const Vec2*>(&triangle.v2);
	const Vec2& vMid = v1.y < v2.y ? v1 : v2;  // y is middle
	const Vec2& vTop = v1.y < v2.y ? v2 : v1;  // y is highest

	const float yMin = std::max(v0.y, 0.0f);
	const float yMax = std::min(vTop.y, static_cast<float>(m_DepthBuffer.Height() - 1));
	const float yMid = std::max(yMin, std::min(yMax, vMid.y));

	// v01 x v02
	const float area = Cross(v1 - v0, v2 - v0);

	// lower half
	{
		float tLeft = (v2.x - v0.x) / (v2.y - v0.y + eps);
		float tRight = (v1.x - v0.x) / (v1.y - v0.y + eps);
		if (tLeft > tRight)
		{
			std::swap(tLeft, tRight);
		}
		for (int y = static_cast<int>(yMin); y < static_cast<int>(yMid); ++y)
		{
			const float yCoord = static_cast<float>(y) + 0.5f;
			const float yDiff = yCoord - v0.y;
			float xMin = tLeft * yDiff + triangle.v0.position.x;
			float xMax = tRight * yDiff + triangle.v0.position.x;
			xMin = std::max(0.0f, xMin);
			xMax = std::min(static_cast<float>(m_DepthBuffer.Width() - 1), xMax);

			for (int x = static_cast<int>(xMin); static_cast<float>(x) <= xMax; ++x)
			{
				// pixel (x, y): coordinate (x + 0.5, y + 0.5)
				const Vec2 p = { static_cast<float>(x) + 0.5f, yCoord };

				// bi-centric coordinates of (xCoord, yCoord) in triangle
				// p-1 x p-2
				const float w0 = Cross(v1 - p, v2 - p) / area;
				// 0-p x 0-2
				const float w1 = Cross(p - v0, v2 - v0) / area;
				// ReSharper disable once CppTooWideScopeInitStatement
				const float w2 = 1 - w0 - w1;

				// fragment shade only inside triangle
				if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				{
					const float z = w0 * triangle.v0.position.z + w1 * triangle.v1.position.z + w2 * triangle.v2.position.z;
					if (float& element = m_DepthBuffer[{y, x}];
						z > element)
					{
						element = z;
					}
				}
			}
		}
	}

	// higher half
	{
		float tLeft = (vMid.x - vTop.x) / (vMid.y - vTop.y + eps);
		float tRight = (v0.x - vTop.x) / (v0.y - vTop.y + eps);
		if (tLeft < tRight)
		{
			std::swap(tLeft, tRight);
		}

		for (int y = static_cast<int>(yMid); y <= static_cast<int>(yMax); ++y)
		{
			const float yCoord = static_cast<float>(y) + 0.5f;
			const float yDiff = yCoord - vTop.y;
			float xMin = tLeft * yDiff + vTop.x;
			float xMax = tRight * yDiff + vTop.x;
			xMin = std::max(0.0f, xMin);
			xMax = std::min(static_cast<float>(m_DepthBuffer.Width() - 1), xMax);

			for (int x = static_cast<int>(xMin); x <= static_cast<int>(xMax); ++x)
			{
				// pixel (x, y): coordinate (x + 0.5, y + 0.5)
				const Vec2 p = { static_cast<float>(x) + 0.5f, yCoord };

				// bi-centric coordinates of (xCoord, yCoord) in triangle
				// p-1 x p-2
				const float w0 = Cross(v1 - p, v2 - p) / area;
				// 0-p x 0-2
				const float w1 = Cross(p - v0, v2 - v0) / area;
				// ReSharper disable once CppTooWideScopeInitStatement
				const float w2 = 1 - w0 - w1;

				// fragment shade only inside triangle
				if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				{
					const float z = w0 * triangle.v0.position.z + w1 * triangle.v1.position.z + w2 * triangle.v2.position.z;
					if (float& element = m_DepthBuffer[{y, x}];
						z > element)
					{
						element = z;
					}
				}
			}
		}
	}

}

void Rasterizer::Render(const Model& model)
{
	for (const Mesh& mesh : model.meshes)
	{
		for (const Triangle& triangle : mesh.triangles)
		{
			Triangle transformed = triangle;
			SortTriangle(transformed);
			TransformToScreen(transformed);
			Rasterize(transformed);
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

