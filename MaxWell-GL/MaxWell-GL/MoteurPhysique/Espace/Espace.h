#pragma once

#include "Lib/GLM/glm/glm.hpp"

struct Espace
{
	union
	{
		glm::ivec4 dimensions;
		struct
		{
			int lx;
			int ly;
			int lz;
			int longueur;
		};
	};

	glm::vec3* pos;
	glm::vec3* force;

	static void Initializer(Espace* carte, glm::ivec3 taille);
	static void Detruire(Espace* carte);
	static void SetPos(Espace* carte, glm::ivec3 index, glm::vec3 valeur);
	static void Remplir(Espace* carte, glm::vec3 min, glm::vec3 max);

};