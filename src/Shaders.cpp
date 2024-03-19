#include "Shaders.h"

#include "Buffer.h"
#include "Model.h"

namespace Islander
{

Vec3 PhongShading(const Material& material, const Vec2& texCoord, const Vec3& normal, const Vec3& lightDir,
	const Vec3& eyeDir)
{
	// sample textures
	Vec3 diffuse = { 0.0f, 0.0f, 0.0f };
	if (material.mapKd != nullptr)
	{
		diffuse = material.mapKd->SampleVec3(texCoord);
	}

	return diffuse;
}

}

