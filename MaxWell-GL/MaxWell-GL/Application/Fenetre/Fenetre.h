#pragma once

#include "Lib/GLM/glm/glm.hpp"
#include "Lib/GLFW/include/GLFW/glfw3.h"

#include "Application/Souris/Souris.h"

struct Fenetre
{
	GLFWwindow* window;

	glm::ivec2 dimension;
	int swapInterval; //Pas utilis�

	Souris souris;

	/** Cr�ation de la fen�tre avec GLFW */
	static int init(Fenetre* fenetre, glm::ivec2 dimension);

	/** Retourner le pointeur pointant � l'objet fen�tre avec le pointeur vers la fen�tre GLFW */
	static Fenetre* ret(GLFWwindow* window);

	/** Actualisation du status de la souris (position, click, ...) */
	static void actualiserSouris(Souris* souris);
};

