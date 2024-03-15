#include "DepthBuffer.h"
#include "Geometry.h"
#include "Rasterizer.h"

int main()
{
	Islander::DepthBuffer renderTarget(1960, 1080);

	std::vector<Islander::ScreenTriangle> triangles = {
		{ { 0.1f, 0.1f, 0.1f }, { 0.9f, 0.4f, 0.9f }, { 0.5f, 0.9f, 0.9f } },
	};
	Rasterize(triangles, renderTarget);

	renderTarget.DumpBmp("tmp.bmp");
}

