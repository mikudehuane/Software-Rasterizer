#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "Math.h"

namespace Islander
{

struct Material
{
	float ns;
	Vec3 ka, kd, ks;
	float ni;
	float d;
	int illum;
	std::string mapKd;
	std::string mapBump;
	std::string mapKs;
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

