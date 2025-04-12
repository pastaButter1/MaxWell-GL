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

	struct DonneesOperation
	{
		float tangageCam = 0.0f;
		float lacetCam = 0.0f;
		float distanceCam = 50.0f;

		float disZ = 0.0f;
	} donnesOperation;

	static void initialiser(Application* const app, glm::uvec2 tailleFenetre);

	static void executer(Application* const app);

	static void fermer(Application* const app);

private:
	static void initaliserMoteurGraphique(Application* const app);

	static void initialiserInterfaceUtilisateur(Application* const app);

	static void executerEntrees(Application* const app, const float dt);

	static void executerRendu(Application* const app);
};