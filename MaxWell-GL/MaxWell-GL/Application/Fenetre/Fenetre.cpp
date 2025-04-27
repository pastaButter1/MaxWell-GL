#include "Moteur Graphique/Graphique.h"	

#include "Fenetre.h"

#include <conio.h>

#include "Application/Utilitaire.h"


int Fenetre::init(Fenetre* fenetre, glm::ivec2 dimension)
{
	fenetre->dimension = dimension;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Création de la fenêtre avec GLFW

	fenetre->window = glfwCreateWindow(fenetre->dimension.x, fenetre->dimension.y, "MaxWell-GL", NULL, NULL);
	if (fenetre->window == nullptr)
	{
		afficherErreur("Erreur dans la creation de la fenetre GLFW");

		glfwTerminate();

		_getch();
		exit(-1);
	}

	glfwMakeContextCurrent(fenetre->window);

	glfwSwapInterval(1);

	afficherLog("Creation de la fenetre avec GLFW");

	glfwMakeContextCurrent(fenetre->window);

	if (glewInit() != GLEW_OK) {
		afficherErreur("L'initialisation de GLEW a echoue");

		_getch();
		exit(-1);
	}

	afficherLog("Initialisation de GLEW");

	afficherLog("OPENGL vendor : %s", glGetString(GL_VENDOR));
	afficherLog("OPENGL renderer : %s", glGetString(GL_RENDERER));
	afficherLog("OPENGL version : %s", glGetString(GL_VERSION));
	afficherLog("OPENGL Supported glsl version : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	fenetre->souris = {};
	glfwSetWindowUserPointer(fenetre->window, (void*)fenetre);
	
	glfwSetMouseButtonCallback(fenetre->window, Souris::callbackBouton);
}

Fenetre* Fenetre::ret(GLFWwindow* window)
{
	return (Fenetre*)glfwGetWindowUserPointer(window);
}