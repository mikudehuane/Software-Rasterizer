#include "Buffer.h"

int main()
{
	Islander::VertexBuffer vb(
		{
			{.name = "Position", .type = Islander::DataType::Float3 },
			{.name = "Color", .type = Islander::DataType::Float4 }
		},
		100
	);
}

