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
	renderTarget.DumpBmp("result.bmp", targetLayout.GetElement("Color"));
}

