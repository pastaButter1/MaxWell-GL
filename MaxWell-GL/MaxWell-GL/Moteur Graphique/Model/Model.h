#pragma once

#include "Lib/GLM/glm/glm.hpp"

#include <stdint.h>

struct Vertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 norm;
};

struct Triangle
{
	Vertex verts[3];
};

struct Model
{
	Triangle* triangles;

	uint32_t nbTriangle;
};