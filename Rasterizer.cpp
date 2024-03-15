#include "Rasterizer.h"

namespace Islander
{

class Rasterizer
{
public:
	Rasterizer(const std::vector<Islander::ScreenTriangle>& triangle, Islander::DepthBuffer& buffer)
		: m_Triangles(triangle), m_Buffer(&buffer)
	{
	}

	void Main()
	{
		SortTriangles();
		RasterizeAll();
	}

	[[nodiscard]] float GetY(const float y) const
	{
		// -1 ~ 1 -> 0 ~ height
		return (y + 1) * 0.5f * static_cast<float>(m_Buffer->Height());
	}

	[[nodiscard]] float GetX(const float x) const
	{
		// -1 ~ 1 -> 0 ~ width
		return (x + 1) * 0.5f * static_cast<float>(m_Buffer->Width());
	}

	void RasterizeAll()
	{
		for (ScreenTriangle& triangle : m_Triangles)
		{
			TransformToScreen(triangle);
			Rasterize(triangle);
		}
	}

	// -1 ~ 1 -> screen coordinate
	void TransformToScreen(ScreenTriangle& triangle) const
	{
		triangle.v0.x = GetX(triangle.v0.x);
		triangle.v0.y = GetY(triangle.v0.y);
		triangle.v1.x = GetX(triangle.v1.x);
		triangle.v1.y = GetY(triangle.v1.y);
		triangle.v2.x = GetX(triangle.v2.x);
		triangle.v2.y = GetY(triangle.v2.y);
	}

	void Rasterize(const ScreenTriangle& triangle) const
	{
		// vertex in the middle and on the top
		const Vec3& vMid = triangle.v1.y < triangle.v2.y ? triangle.v1 : triangle.v2;
		const Vec3& vTop = triangle.v1.y < triangle.v2.y ? triangle.v2 : triangle.v1;

		const Vec2& v0 = *reinterpret_cast<const Vec2*>(&triangle.v0);
		const Vec2& v1 = *reinterpret_cast<const Vec2*>(&triangle.v1);
		const Vec2& v2 = *reinterpret_cast<const Vec2*>(&triangle.v2);

		const float yMin = triangle.v0.y;
		const float yMid = vMid.y;
		const float yMax = vTop.y;

		// 1 / slope
		const float tLeft = (v2.x - v0.x) / (v2.y - v0.y);
		const float tRight = (v1.x - v0.x) / (v1.y - v0.y);
		assert(tLeft <= tRight);
		// v01 x v02
		const float area = Cross(v1 - v0, v2 - v0);
		// find left-most and right-most pixels for each row
		// pixel (x, y): coordinate (x + 0.5, y + 0.5)
		for (int y = static_cast<int>(yMin); static_cast<float>(y) <= yMid; ++y)
		{
			const float yCoord = static_cast<float>(y) + 0.5f;
			const float yDiff = yCoord - yMin;
			const float xMin = tLeft * yDiff + triangle.v0.x;
			const float xMax = tRight * yDiff + triangle.v0.x;
			for (int x = static_cast<int>(xMin); static_cast<float>(x) <= xMax; ++x)
			{
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
					const float z = w0 * triangle.v0.z + w1 * triangle.v1.z + w2 * triangle.v2.z;
					(*m_Buffer)[{y, x}] = z;
				}
			}
		}
	}

	void SortTriangles()
	{
		for (ScreenTriangle& triangle : m_Triangles)
		{
			// v0 at bottom
			{
				if (triangle.v0.y > triangle.v1.y)
				{
					std::swap(triangle.v0, triangle.v1);
				}
				if (triangle.v0.y > triangle.v2.y)
				{
					std::swap(triangle.v0, triangle.v2);
				}
			}

			// arrange v0-v1-v2 to be counter-clockwise
			if (Cross(
				{ triangle.v1.x - triangle.v0.x, triangle.v1.y - triangle.v0.y },
				{ triangle.v2.x - triangle.v0.x, triangle.v2.y - triangle.v0.y }
			) < 0)
			{
				std::swap(triangle.v1, triangle.v2);
			}
		}
	}
private:
	std::vector<ScreenTriangle> m_Triangles;
	DepthBuffer* m_Buffer;
};

void Rasterize(const std::vector<ScreenTriangle>& triangle, DepthBuffer& buffer)
{
	Rasterizer rasterizer(triangle, buffer);
	rasterizer.Main();
}

}

