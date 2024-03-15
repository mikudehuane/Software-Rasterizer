#include "DepthBuffer.h"
#include "Geometry.h"
#include "Rasterizer.h"

int main()
{
	Islander::DepthBuffer renderTarget(1960, 1080);

	std::vector<Islander::ScreenTriangle> triangles = {
		{ { 0.1f, 0.1f, 0.1f }, { 0.9f, 0.4f, 0.9f }, { 0.5f, 0.9f, 0.9f } },
		{ { -1.2f, -1.2f, -1.0f }, { 1.1f, 1.3f, 0.5f }, { 2.0f, 1.2f, -0.5f } }
	};
	Rasterize(triangles, renderTarget);

	renderTarget.DumpBmp("tmp.bmp");
}

