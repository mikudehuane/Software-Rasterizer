#include "Buffer.h"
#include "DepthBuffer.h"

int main()
{
	const Islander::BufferLayout bufferLayout({
		{.name = "Position", .type = Islander::DataType::Float, .size = 4, .offset = 0 },
		{.name = "Color", .type = Islander::DataType::Float, .size = 3, .offset = 16}
	}, 28);
	const Islander::BufferLayout vertexAttachment({
		bufferLayout.GetElement("Color")
	}, bufferLayout.GetStride());
	const Islander::BufferLayout targetLayout = vertexAttachment.Packed();

	Islander::VertexBuffer buffer(bufferLayout, 100);
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

