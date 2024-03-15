#pragma once

#include "DepthBuffer.h"
#include "Geometry.h"

namespace Islander
{

void Rasterize(const std::vector<ScreenTriangle>& triangle, DepthBuffer& buffer);
	
}

