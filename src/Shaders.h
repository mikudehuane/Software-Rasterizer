#pragma once

#include "Math.h"

namespace Islander
{

struct Material;

Vec3 PhongShading(
    const Material& material,
    // vertex data
    const Vec2& texCoord, const Vec3& normal, const Vec3& viewDir
);
	
}

