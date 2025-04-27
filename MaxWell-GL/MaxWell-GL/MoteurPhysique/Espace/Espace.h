#pragma once

#include "Lib/GLM/glm/glm.hpp"
#include "Moteur Graphique/Texture/Texture.h"

struct Espace
{
	union
	{
		glm::ivec4 dimensions;
		struct
		{
			int32_t lx;
			int32_t ly;
			int32_t lz;
			int32_t longueur;
		};
	};

	glm::vec3* tableau;

	static void generer(Espace* const espace, const glm::ivec3 dimension);
	static void detruire(Espace * const espace);
	static void setPos(Espace * const espace, const glm::ivec3 index, const glm::vec3 valeur);
	static void remplir(Espace * const espace, const glm::vec3 min, const glm::vec3 max);

	//GPU
	Texture tex;
	struct GPU
	{
		static void initialiser(Espace* const espace, const glm::ivec3 dimension);
		static void Recuperer(Espace* const espace);
		static void soumettre(Espace* const espace);
		static void Utiliser(const Espace& espace);
	};
};