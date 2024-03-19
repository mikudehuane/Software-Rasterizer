#include "Shaders.h"

#include "Buffer.h"
#include "Model.h"

namespace Islander
{

Vec3 PhongShading(const Material& material, const Vec2& texCoord, const Vec3& normal, const Vec3& viewDir)
{
	// uniforms (hard-coded)
	constexpr float lightAmbient = 0.3f;
	constexpr float lightDiffuse = 3.0f;
	constexpr float lightSpecular = 3.0f;
	constexpr Vec3 lightColor = { 1.0f, 1.0f, 1.0f };
	//constexpr Vec3 lightDir = { 0.0f, 0.0f, -1.0f };
	constexpr Vec3 lightDir = { -0.18814417f, -0.94072087f, -0.28221626f };

	Vec3 texDiffuse = { 0.0f, 0.0f, 0.0f };
	if (material.mapKd != nullptr)
	{
		texDiffuse = material.mapKd->Sample(texCoord).ToVec3();
	}
	Vec3 texSpecular = { 0.0f, 0.0f, 0.0f };
	if (material.mapKs != nullptr)
	{
		texSpecular = material.mapKs->Sample(texCoord).ToVec3();
	}

	const Vec3 ambient = lightAmbient * lightColor * texDiffuse;

	const float diffuseIntensity = std::max(Dot(normal, -lightDir), 0.0f);
	const Vec3 diffuse = diffuseIntensity * lightDiffuse * lightColor * texDiffuse;

	const Vec3 reflectDir = Reflect(lightDir, normal);
	const float specularIntensity = std::pow(
		std::max(Dot(viewDir, reflectDir), 0.0f),
		material.ns);
	const Vec3 specular = specularIntensity * lightSpecular * lightColor * texSpecular;

	return ambient + diffuse + specular;
}

}

