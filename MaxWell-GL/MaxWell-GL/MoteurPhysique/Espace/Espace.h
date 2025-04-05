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

	static void Initialiser(Espace* const espace, const glm::ivec3 dimension);
	static void Detruire(Espace * const espace);
	static void SetPos(Espace * const espace, const glm::ivec3 index, const glm::vec3 valeur);
	static void Remplir(Espace * const espace, const glm::vec3 min, const glm::vec3 max);

	//GPU
	Texture tex;

	static void gpuInitialiser(Espace* const espace, const glm::ivec3 dimension);
	static void gpuRecuperer(Espace* const espace);
	static void gpuSoumettre(const Espace& espace);
	static void gpuUtiliser(const Espace& espace);
	static void gpuCalculer(const Espace& espace);
};