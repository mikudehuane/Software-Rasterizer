#include "DepthBuffer.h"

int main()
{
	Islander::DepthBuffer renderTarget(100, 100);
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			renderTarget[{i, j}] = i * 0.005f + j * 0.005f;
		}
	}
	renderTarget.DumpBmp("result.bmp");
}

