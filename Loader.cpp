#include "Loader.h"

#include <fstream>
#include <iostream>
#include <regex>

namespace Islander
{
	
// read vertex data from obj
std::vector<ScreenTriangle> LoadMesh(const std::string& path)
{
	std::vector<Vec3> vertices;
	std::vector<ScreenTriangle> triangles;

	std::ifstream fin(path);

	std::string line;

	std::regex faceRegex(R"(f (\d+)/\d+/\d+ (\d+)/\d+/\d+ (\d+)/\d+/\d+)");
	std::regex vertexRegex(R"(v (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+))");
	while (std::getline(fin, line))
	{
		if (std::smatch match; std::regex_match(line, match, vertexRegex))
		{
			vertices.push_back({ std::stof(match[1]), std::stof(match[2]), std::stof(match[3]) });
		}

		if (std::smatch match; std::regex_match(line, match, faceRegex))
		{
			ScreenTriangle triangle;
			triangle.v0 = vertices[std::stoi(match[1]) - 1];
			triangle.v1 = vertices[std::stoi(match[2]) - 1];
			triangle.v2 = vertices[std::stoi(match[3]) - 1];
			triangles.push_back(triangle);
		}
	}

	return triangles;
}

}
