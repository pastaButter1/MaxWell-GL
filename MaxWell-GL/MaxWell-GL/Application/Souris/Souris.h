#pragma once

#include "Lib/GLM/glm/glm.hpp"
#include "Lib/GLFW/include/GLFW/glfw3.h"

struct Souris
{
	glm::dvec2 position;
	glm::dvec2 deplacement;

	static void callbackBouton(GLFWwindow* window, int bouton, int action, int modes);
};