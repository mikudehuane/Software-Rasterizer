#include "DepthBuffer.h"
#include "Geometry.h"
#include "Rasterizer.h"

int main()
{
	Islander::DepthBuffer renderTarget(100, 100);

	std::vector<Islander::ScreenTriangle> triangles = {
		{ { 10, 10, 0 }, { 90, 10, 10 }, { 50, 90, 20 } },
		{ { 10, 10, 0 }, { 50, 90, 20 }, { 90, 10, 10 } }
	};
	Rasterize(triangles, renderTarget);

	renderTarget.DumpBmp("tmp.bmp");
}

