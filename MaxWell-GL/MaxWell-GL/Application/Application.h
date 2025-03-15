#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLFW/include/GLFW/glfw3.h"
#include "Lib/IMGUI/IMGUI/imgui.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"

struct Application {
	glm::uvec2 tailleFenetre;
	GLFWwindow* fenetre = nullptr;
	ImFont* police;
	MoteurGX moteurGX;

	static void initialiser(Application* const app, glm::uvec2 tailleFenetre);

	static void executer(const Application& app);

	static void fermer(Application* const app);
};