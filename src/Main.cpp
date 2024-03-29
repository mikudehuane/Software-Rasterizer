#include <filesystem>
#include <iostream>

#include "Buffer.h"
#include "Model.h"
#include "Rasterizer.h"

void Normalize(Islander::Model& model)
{
	// check the x, y, z range
	//float xMin = std::numeric_limits<float>::max();
	//float xMax = std::numeric_limits<float>::min();
	//float yMin = std::numeric_limits<float>::max();
	//float yMax = std::numeric_limits<float>::min();
	//float zMin = std::numeric_limits<float>::max();
	//float zMax = std::numeric_limits<float>::min();
	//for (const Islander::Mesh& mesh : model.meshes)
	//{
	//	for (const Islander::Triangle& triangle : mesh.triangles)
	//	{
	//		xMin = std::min({ xMin, triangle.v0.position.x, triangle.v1.position.x, triangle.v2.position.x });
	//		xMax = std::max({ xMax, triangle.v0.position.x, triangle.v1.position.x, triangle.v2.position.x });
	//		yMin = std::min({ yMin, triangle.v0.position.y, triangle.v1.position.y, triangle.v2.position.y });
	//		yMax = std::max({ yMax, triangle.v0.position.y, triangle.v1.position.y, triangle.v2.position.y });
	//		zMin = std::min({ zMin, triangle.v0.position.z, triangle.v1.position.z, triangle.v2.position.z });
	//		zMax = std::max({ zMax, triangle.v0.position.z, triangle.v1.position.z, triangle.v2.position.z });
	//	}
	//}
	//std::cout << "x: " << xMin << " ~ " << xMax << std::endl;
	//std::cout << "y: " << yMin << " ~ " << yMax << std::endl;
	//std::cout << "z: " << zMin << " ~ " << zMax << std::endl;
	constexpr float xMin = -4.0f;
	constexpr float xMax = 4.0f;
	constexpr float yMin = 0.0f;
	constexpr float yMax = 16.0f;
	constexpr float zMin = -1.72f;
	constexpr float zMax = 1.72f;

	constexpr float extent = std::max({ xMax - xMin, yMax - yMin });

	// normalize x, y to -1 ~ 1
	auto normalize = [xMin, xMax, yMin, yMax, zMin, zMax, extent](Islander::Vec3& p)
		{
			p.x = (p.x - (xMin + xMax) / 2) / (extent / 2);
			p.y = (p.y - (yMin + yMax) / 2) / (extent / 2);
			p.z = (p.z - (zMin + zMax) / 2) / ((zMax - zMin) / 2);
		};
	for (Islander::Mesh& mesh : model.meshes)
	{
		for (Islander::Triangle& triangle : mesh.triangles)
		{
			normalize(triangle.v0.position);
			normalize(triangle.v1.position);
			normalize(triangle.v2.position);
		}
	}
}

int main(int argc, char* argv[])
{
	Islander::Model model = Islander::LoadModel("assets/nanosuit.obj");

	Normalize(model);

	constexpr int resolution = 4096;
	Islander::Rasterizer rasterizer(resolution, resolution);
	rasterizer.Render(model);
	//rasterizer.GetDepthBuffer().DumpBmp("depth.bmp");
	rasterizer.GetColorBuffer().DumpBmp("result.bmp");
}

