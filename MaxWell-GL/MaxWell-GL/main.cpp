#include <cstdio>
#include <conio.h>

#include "Graphique.h"

#include "Lib/GLFW/include/GLFW/glfw3.h"

int main() {
	printf("allo le monde!");

	uint32_t width = 800;
	uint32_t height = 600;

	// Initialisation de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Création de la fenêtre avec GLFW

	GLFWwindow* window = glfwCreateWindow(width, height, "OPENGL 3D", NULL, NULL);
	if (window == NULL) {
		printf("ERREUR | Erreur dans la création de la fenetre GLFW\n");

		glfwTerminate();

		_getch();
		return -1;
	}

	printf("LOG | Creation de la fenetre avec GLFW\n");

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// Initialisation de GLEW

	if (glewInit() != GLEW_OK) {
		printf("ERREUR | Erreur dans l'initialisation de GLEW\n");

		_getch();
		return -1;
	}

	printf("LOG | Initialisation de GLEW\n");

	printf("LOG | OPENGL vendor : %s\n", glGetString(GL_VENDOR));
	printf("LOG | OPENGL renderer : %s\n", glGetString(GL_RENDERER));
	printf("LOG | OPENGL version : %s\n", glGetString(GL_VERSION));
	printf("LOG | OPENGL Supported glsl version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwTerminate();

	_getch();

	return -1;
}