#pragma once

#include "Application/Fenetre/Fenetre.h"
#include "Lib/GLM/glm/gtc/matrix_transform.hpp"

struct Camera
{
	glm::vec2 rotation;
	glm::vec3 position;

	glm::mat4 plan;

	static void input(Camera* camera, Fenetre* fenetre, float deltaTemps);

	static void transformer(Camera* camera);
};