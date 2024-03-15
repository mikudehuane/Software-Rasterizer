#include <iostream>

#include "DepthBuffer.h"
#include "Geometry.h"
#include "Loader.h"
#include "Rasterizer.h"

void Normalize(std::vector<Islander::ScreenTriangle>& triangles)
{
	// check the x, y, z range
	float xMin = std::numeric_limits<float>::max();
	float xMax = std::numeric_limits<float>::min();
	float yMin = std::numeric_limits<float>::max();
	float yMax = std::numeric_limits<float>::min();
	float zMin = std::numeric_limits<float>::max();
	float zMax = std::numeric_limits<float>::min();
	for (const Islander::ScreenTriangle& triangle : triangles)
	{
		xMin = std::min({ xMin, triangle.v0.x, triangle.v1.x, triangle.v2.x });
		xMax = std::max({ xMax, triangle.v0.x, triangle.v1.x, triangle.v2.x });
		yMin = std::min({ yMin, triangle.v0.y, triangle.v1.y, triangle.v2.y });
		yMax = std::max({ yMax, triangle.v0.y, triangle.v1.y, triangle.v2.y });
		zMin = std::min({ zMin, triangle.v0.z, triangle.v1.z, triangle.v2.z });
		zMax = std::max({ zMax, triangle.v0.z, triangle.v1.z, triangle.v2.z });
	}
	std::cout << "x: " << xMin << " ~ " << xMax << std::endl;
	std::cout << "y: " << yMin << " ~ " << yMax << std::endl;
	std::cout << "z: " << zMin << " ~ " << zMax << std::endl;
	const float extent = std::max({ xMax - xMin, yMax - yMin });

	// normalize x, y to -1 ~ 1
	auto normalize = [xMin, xMax, yMin, yMax, zMin, zMax, extent](Islander::Vec3& p)
		{
			p.x = (p.x - (xMin + xMax) / 2) / (extent / 2);
			p.y = (p.y - (yMin + yMax) / 2) / (extent / 2);
			p.z = (p.z - (zMin + zMax) / 2) / ((zMax - zMin) / 2);
		};
	for (Islander::ScreenTriangle& triangle : triangles)
	{
		normalize(triangle.v0);
		normalize(triangle.v1);
		normalize(triangle.v2);
	}
}

int main()
{
	std::vector<Islander::ScreenTriangle> triangles = Islander::LoadMesh("Assets/nanosuit.obj");

	Normalize(triangles);

	Islander::DepthBuffer renderTarget(720, 720);

	Rasterize(triangles, renderTarget);

	renderTarget.DumpBmp("tmp.bmp");
}

