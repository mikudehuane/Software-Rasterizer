#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "Math.h"

namespace Islander
{

class ColorBuffer;

struct Material
{
	// specular exponent
	float ns;
	Vec3 ka;
	std::shared_ptr<ColorBuffer> mapKd;
	std::shared_ptr<ColorBuffer> mapKs;
};

struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec2 texCoord;
};

struct Triangle
{
	Vertex v0, v1, v2;
};

struct Mesh
{
	std::vector<Triangle> triangles;
	std::shared_ptr<Material> material;
};

struct Model
{
	std::vector<Mesh> meshes;
};
	
Model LoadModel(const std::string& path);

}

