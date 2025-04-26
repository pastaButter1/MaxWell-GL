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

	static void initialiser(Application* const app, glm::uvec2 tailleFenetre);

	static void executer(Application* const app);

	static void executerSimulation(Application* const app, const float dt);

	static void fermer(Application* const app);

private:
	static void initaliserMoteurGraphique(Application* const app);

	static void initialiserInterfaceUtilisateur(Application* const app);
};