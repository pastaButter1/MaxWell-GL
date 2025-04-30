#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLFW/include/GLFW/glfw3.h"
#include "Lib/IMGUI/IMGUI/imgui.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"
#include "Fenetre/Fenetre.h"
#include "Moteur Graphique/MoteurGx/Mesh.h"
#include "MoteurPhysique/MoteurPhysique/MoteurPhysique.h"

struct Application {
	Fenetre fenetre;
	ImFont* police;
	MoteurGX moteurGX;
	MoteurPhysique moteurPhysique;

	struct DonneesOperation
	{
		float tangageCam = 0.0f;
		float lacetCam = 0.0f;
		float distanceCam = 50.0f;

		float disZ = 0.0f;

		glm::vec3 camPos = glm::vec3(0.0f);
		glm::vec2 angleLumiere = glm::vec2(0.0f);

		struct Solenoide
		{
			float longueur = 10.0f;
			float nbRot = 10.0f;
			float r = 0.5f;
			float R = 5.0f;
		} solenoide;

		mgx::Mesh meshSolenoide, meshCube;

		float kAmbient = 0.110f;
		float kDiffuse = 0.260f;
		float kSpeculaire = 1.0f;
		float esposantSpec = 0.734f;
		glm::vec3 couleur = glm::vec3(1.0f, 0.230f, 0.020f);

		mgx::Ressource skyboxIU;

	} donnesOperation;

	static void initialiser(Application* const app, glm::uvec2 tailleFenetre);

	static void executer(Application* const app);

	static void fermer(Application* const app);

private:
	static void initialiserMoteurGraphique(Application* const app);

	static void initialiserSimulation(Application* const app);

	static void initialiserInterfaceUtilisateur(Application* const app);

	static void executerEntrees(Application* const app, const float dt);

	static void executerRendu(Application* const app);

	static void executerSimulation(Application* const app);
};