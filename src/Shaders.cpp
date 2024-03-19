#include "Shaders.h"

#include "Buffer.h"
#include "Model.h"

namespace Islander
{

Vec3 PhongShading(const Material& material, const Vec2& texCoord, const Vec3& normal)
{
	// sample textures
	Vec3 diffuse = { 0.0f, 0.0f, 0.0f };
	if (material.mapKd != nullptr)
	{
		diffuse = material.mapKd->Sample(texCoord).ToVec3();
	}

	return diffuse;
}

}

