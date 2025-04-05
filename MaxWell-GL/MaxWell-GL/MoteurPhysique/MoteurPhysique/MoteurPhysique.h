#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include "Moteur Graphique/Shader/Shader.h"
#include "MoteurPhysique/Espace/Espace.h"
#include "Moteur Graphique/MoteurGx/MoteurGXDef.h"

struct MoteurPhysique
{
	Espace coordonnees;
	Espace champMagnetique;

	Shader shaderCoordonnees;
	Shader shaderChampMagnetique;

	struct alignas(16) Fil
	{
		glm::vec3 direction;
		float courant;
		glm::vec3 origine;
	};

	struct Info
	{
		std::vector<Fil> fils;
		GLuint buffer;
	};

	static void ChargerShaders(MoteurPhysique* const moteurPhysique, const std::string& cheminDossier);
	static void InitialiserFils(Info* const info);
	static void SoumettreFils(const Info& info);
	static void CalculerGPU(const Shader& shader, const Espace & coordonnees, const Espace& donnees, const Info& info);
	static void AssignerCoordonnees(const MoteurPhysique& moteurPhysique, glm::vec3 min, glm::vec3 max);
};