#include "Rasterizer.h"

namespace Islander
{

class Rasterizer
{
public:
	Rasterizer(const std::vector<Islander::ScreenTriangle>& triangle, Islander::DepthBuffer& buffer)
		: m_Triangles(triangle), m_Buffer(buffer)
	{
	}

	void Rasterize()
	{
		SortTriangles();
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
			if ((triangle.v1.x - triangle.v0.x) * (triangle.v2.y - triangle.v0.y) -
				(triangle.v1.y - triangle.v0.y) * (triangle.v2.x - triangle.v0.x) < 0)
			{
				std::swap(triangle.v1, triangle.v2);
			}
		}
	}
private:
	std::vector<ScreenTriangle> m_Triangles;
	DepthBuffer& m_Buffer;
};

void Rasterize(const std::vector<ScreenTriangle>& triangle, DepthBuffer& buffer)
{
	Rasterizer rasterizer(triangle, buffer);
	rasterizer.Rasterize();
}

}

