#pragma once

#include "Lib/GLM/glm/glm.hpp"
#include "Lib/GLFW/include/GLFW/glfw3.h"

#include "Application/Souris/Souris.h"

struct Fenetre
{
	GLFWwindow* window;

	glm::ivec2 dimension;
	int swapInterval; //Pas utilisé

	Souris souris;

	static int init(Fenetre* fenetre, glm::ivec2 dimension);
	static Fenetre* ret(GLFWwindow* window);

	static void actualiserSouris(Souris* souris);
};

