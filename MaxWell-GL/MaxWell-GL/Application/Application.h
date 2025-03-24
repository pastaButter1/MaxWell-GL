#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLFW/include/GLFW/glfw3.h"
#include "Lib/IMGUI/IMGUI/imgui.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"
#include "Fenetre/Fenetre.h"

struct Application {
	Fenetre fenetre;
	ImFont* police;
	MoteurGX moteurGX;

	static void initialiser(Application* const app, glm::ivec2 tailleFenetre);

	static void executer(const Application& app);

	static void fermer(Application* const app);
};