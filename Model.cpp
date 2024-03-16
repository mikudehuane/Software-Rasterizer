#include "Model.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Math.h"
#include "Buffer.h"

namespace Islander
{

static std::shared_ptr<ColorBuffer> LoadPngTexture(const std::filesystem::path& path)
{
	int width, height, nChannel;
	const std::string p = path.string();
	unsigned char* data = stbi_load(p.c_str(), &width, &height, &nChannel, 0);
	if (!data)
	{
		std::cerr << "Error: Unable to load file " << path << std::endl;
		return nullptr;
	}
	assert(nChannel == 3 || nChannel == 4);

	auto buffer = std::make_shared<ColorBuffer>(width, height);
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			const int index = (i * width + j) * nChannel;
			Color color(
				data[index], data[index + 1], data[index + 2], 255
			);
			if (nChannel == 4)
			{
				color.a = data[index + 3];
			}
			(*buffer)[{i, j}] = color;
		}
	}

	stbi_image_free(data);
	return buffer;
}

using MaterialLib = std::map<std::string, std::shared_ptr<Material>>;

static MaterialLib LoadMaterialLib(const std::filesystem::path& path)
{
	MaterialLib lib;
	std::regex newmtlRegex(R"(newmtl (.+))");
	std::regex nsRegex(R"(Ns (\d+\.\d+))");
	std::regex kaRegex(R"(Ka (\d+\.\d+) (\d+\.\d+) (\d+\.\d+))");
	std::regex kdRegex(R"(Kd (\d+\.\d+) (\d+\.\d+) (\d+\.\d+))");
	std::regex ksRegex(R"(Ks (\d+\.\d+) (\d+\.\d+) (\d+\.\d+))");
	std::regex niRegex(R"(Ni (\d+\.\d+))");
	std::regex dRegex(R"(d (\d+\.\d+))");
	std::regex illumRegex(R"(illum (\d+))");
	std::regex mapKdRegex(R"(map_Kd (.+))");
	std::regex mapBumpRegex(R"(map_Bump (.+))");
	std::regex mapKsRegex(R"(map_Ks (.+))");

	std::ifstream fin(path);
	std::string line;
	std::shared_ptr<Material> current = nullptr;
	while (std::getline(fin, line))
	{
		if (line.starts_with("newmtl "))
		{
			std::smatch match;
			std::regex_match(line, match, newmtlRegex);
			assert(match.size() == 2);
			current = std::make_shared<Material>();
			lib[match[1]] = current;
		}
		else if (line.starts_with("Ns "))
		{
			std::smatch match;
			std::regex_match(line, match, nsRegex);
			assert(match.size() == 2);
			current->ns = std::stof(match[1]);
		}
		else if (line.starts_with("Ka "))
		{
			std::smatch match;
			std::regex_match(line, match, kaRegex);
			assert(match.size() == 4);
			current->ka = { std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) };
		}
		else if (line.starts_with("Kd "))
		{
			std::smatch match;
			std::regex_match(line, match, kdRegex);
			assert(match.size() == 4);
			current->kd = { std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) };
		}
		else if (line.starts_with("Ks "))
		{
			std::smatch match;
			std::regex_match(line, match, ksRegex);
			assert(match.size() == 4);
			current->ks = { std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) };
		}
		else if (line.starts_with("Ni "))
		{
			std::smatch match;
			std::regex_match(line, match, niRegex);
			assert(match.size() == 2);
			current->ni = std::stof(match[1]);
		}
		else if (line.starts_with("d "))
		{
			std::smatch match;
			std::regex_match(line, match, dRegex);
			assert(match.size() == 2);
			current->d = std::stof(match[1]);
		}
		else if (line.starts_with("illum "))
		{
			std::smatch match;
			std::regex_match(line, match, illumRegex);
			assert(match.size() == 2);
			current->illum = std::stoi(match[1]);
		}
		else if (line.starts_with("map_Kd "))
		{
			std::smatch match;
			std::regex_match(line, match, mapKdRegex);
			assert(match.size() == 2);
			current->mapKd = LoadPngTexture(path.parent_path() / match[1].str());
		}
		else if (line.starts_with("map_Bump "))
		{
			std::smatch match;
			std::regex_match(line, match, mapBumpRegex);
			assert(match.size() == 2);
			current->mapBump = LoadPngTexture(path.parent_path() / match[1].str());
		}
		else if (line.starts_with("map_Ks "))
		{
			std::smatch match;
			std::regex_match(line, match, mapKsRegex);
			assert(match.size() == 2);
			current->mapKs = LoadPngTexture(path.parent_path() / match[1].str());
		}
	}

	return lib;
}


// read vertex data from obj
Model LoadModel(const std::string& path)
{
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texCoords;
	MaterialLib table;
	Model model;

	std::ifstream fin(path);
	if (!fin)
	{
		std::cerr << "Error: Unable to open file " << path << std::endl;
		return model;
	}

	std::string line;

	std::regex faceRegex(R"(f (\d+)/(\d+)/(\d+) (\d+)/(\d+)/(\d+) (\d+)/(\d+)/(\d+))");
	std::regex positionRegex(R"(v (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+))");
	std::regex normalRegex(R"(vn (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+))");
	std::regex texCoordRegex(R"(vt (-?\d+\.\d+) (-?\d+\.\d+))");
	std::regex materialLibRegex(R"(mtllib (.+))");
	std::regex meshRegex(R"(o (.+))");
	std::regex usemtlRegex(R"(usemtl (.+))");

	Mesh* currentMesh = nullptr;
	while (std::getline(fin, line))
	{
		if (line.starts_with("mtllib "))
		{
			std::smatch match;
			std::regex_match(line, match, materialLibRegex);
			assert(match.size() == 2);
			std::filesystem::path p = path;
			p = p.parent_path() / match[1].str();
			table = LoadMaterialLib(p);
		}
		else if (line.starts_with("v "))
		{
			std::smatch match;
			std::regex_match(line, match, positionRegex);
			assert(match.size() == 4);
			positions.push_back({ std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) });
		}
		else if (line.starts_with("vn "))
		{
			std::smatch match;
			std::regex_match(line, match, normalRegex);
			assert(match.size() == 4);
			normals.push_back({ std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) });
		}
		else if (line.starts_with("vt "))
		{
			std::smatch match;
			std::regex_match(line, match, texCoordRegex);
			assert(match.size() == 3);
			texCoords.push_back({ std::stof(match[1]), std::stof(match[2]) });
		}
		else if (line.starts_with("o "))
		{
			std::smatch match;
			std::regex_match(line, match, meshRegex);
			assert(match.size() == 2);
			model.meshes.emplace_back();
			currentMesh = &model.meshes.back();
		}
		else if (line.starts_with("usemtl "))
		{
			std::smatch match;
			std::regex_match(line, match, usemtlRegex);
			assert(match.size() == 2);
			currentMesh->material = table[match[1]];
		}
		else if (line.starts_with("f "))
		{
			assert(currentMesh);
			std::smatch match;
			std::regex_match(line, match, faceRegex);
			assert(match.size() == 10);
			currentMesh->triangles.push_back({
				{
					positions[std::stoi(match[1]) - 1],
					normals[std::stoi(match[3]) - 1],
					texCoords[std::stoi(match[2]) - 1]
				},
				{
					positions[std::stoi(match[4]) - 1],
					normals[std::stoi(match[6]) - 1],
					texCoords[std::stoi(match[5]) - 1],
				},
				{
					positions[std::stoi(match[7]) - 1],
					normals[std::stoi(match[9]) - 1],
					texCoords[std::stoi(match[8]) - 1]
				}
			});
		}
	}

	return model;
}

}
