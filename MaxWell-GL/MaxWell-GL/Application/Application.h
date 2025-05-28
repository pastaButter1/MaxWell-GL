#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLFW/include/GLFW/glfw3.h"
#include "Lib/IMGUI/IMGUI/imgui.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"
#include "Fenetre/Fenetre.h"
#include "Moteur Graphique/MoteurGx/Mesh.h"
#include "MoteurPhysique/MoteurPhysique/MoteurPhysique.h"

#include <string>

/**
* 
* Classe de base du projet
* 
*/
struct Application {
	Fenetre fenetre;
	ImFont* police;
	MoteurGX moteurGX;
	MoteurPhysique moteurPhysique;
	std::string dossierExecution;

	/**
	* Structure contenant toutes les informations pouvant �tre modifi�es par l'utilisateur � l'aide de l'interface graphique
	*/
	struct DonneesOperation
	{
		float tangageCam = 0.0f;
		float lacetCam = 0.0f;
		float distanceCam = 50.0f;

		/** Distance du plan par rapport au plan ZY(oui le nom de la variable est mauvais) */
		float disZ = 0.0f;

		glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec2 angleLumiere = glm::vec2(325.0f, 44.0f);


		/** Param�tres du sol�no�de */
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

		struct FDM
		{
			mgx::Ressource carte1IU;
			float vin = 1, vout = 0, c = 0;
		} fdm;

	} donnesOperation;

	/** Fonction cr�ant la f�n�tre et d�marant les autres routines de d�marrage */
	static void initialiser(Application* const app, glm::uvec2 tailleFenetre);

	/** Fonction raffraichissant le programme */
	static void executer(Application* const app);

	/** Fonction d�truisant toutes les ressources du programme pour la fin de l'�x�cution */
	static void fermer(Application* const app);

private:

	/** D�marrage de l'interface avec OPENGL */
	static void initialiserMoteurGraphique(Application* const app);

	/** Intialisation des ressources n�cessaire � la simulation */
	static void initialiserSimulation(Application* const app);

	/** Intialisation l'interface graphique (GUI) */
	static void initialiserInterfaceUtilisateur(Application* const app);

	/** Lecture des entr�es (input) */
	static void executerEntrees(Application* const app, const float dt);

	/** �x�cution des routines graphique sur le GPU et raffraichissement de l'�cran */
	static void executerRendu(Application* const app);

	/** Actualisation de la simulation avec les param�tres actualis�s */
	static void executerSimulation(Application* const app);
};