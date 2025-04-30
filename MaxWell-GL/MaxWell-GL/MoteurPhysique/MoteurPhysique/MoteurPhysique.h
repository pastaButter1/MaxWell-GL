#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include "Moteur Graphique/Shader/Shader.h"
#include "MoteurPhysique/Espace/Espace.h"
#include "Moteur Graphique/MoteurGx/MoteurGXDef.h"

struct MoteurPhysique
{
	struct alignas(16) Fil
	{
		glm::vec3 direction;
		float courant;
		glm::vec3 origine;

		static Fil creer(glm::vec3 direction, glm::vec3 origine, float courant);
	};

	struct alignas(16) Solenoide
	{
		glm::vec3 direction;
		float courant;
		glm::vec3 origine;
		float nombreTours;
		float longeur;

		static Solenoide creer(glm::vec3 direction, glm::vec3 origine, float courant, float nombreTours, float longeur);
	};

	struct Info
	{
		GLuint buffers[2];
		std::vector<Fil> fils;
		std::vector<Solenoide> solenoides;
	};

	Espace coordonnees;
	Espace champMagnetique;

	Shader shaderCoordonnees;
	Shader shaderChampMagnetique;

	mgx::Ressource vao;
	mgx::Ressource fbo;
	mgx::Ressource texFbo;
	mgx::Ressource texGradient;

	mgx::Ressource pipeline;

	Info info;

	struct GPU
	{
		static void chargerShaders(MoteurPhysique* const moteurPhysique, const std::string& cheminDossier);
		static void genererBufferInfo(MoteurPhysique::Info* const info);
		static void soumettreBufferInfo(const MoteurPhysique::Info& info);
		static void executerCalcul(const Shader& shader, const Espace & coordonnees, const Espace& donnees, const MoteurPhysique::Info& info);
		static void assignerCoordonnees(const MoteurPhysique& moteurPhysique, glm::vec3 min, glm::vec3 max);
	};
};