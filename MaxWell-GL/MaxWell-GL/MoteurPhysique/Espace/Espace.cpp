#include "Espace.h"

void Espace::Initializer(Espace* carte, glm::ivec3 taille)
{
	carte->dimensions = glm::ivec4(taille, taille.x * taille.y * taille.z);
	carte->longueur = taille.x * taille.y * taille.z;
	carte->pos = new glm::vec3[2 * carte->longueur];
	carte->force = &carte->pos[carte->longueur];
}

void Espace::Detruire(Espace* carte)
{
	delete[] carte->pos;
}

void Espace::SetPos(Espace* carte, glm::ivec3 index, glm::vec3 valeur)
{
	carte->pos[index.z * carte->ly * carte->lx + index.y * carte->lx + index.x] = valeur;
}

void Espace::Remplir(Espace* carte, glm::vec3 min, glm::vec3 max)
{
	glm::vec3 scalaire = (max - min) / glm::vec3(carte->dimensions);
	glm::ivec3 index = { 0,0,0 };

	for (index.z = 0; index.z < carte->lz; index += glm::ivec3(0, 0, 1))
	{
		for (index.y = 0; index.y < carte->ly; index += glm::ivec3(0, 1, 0))
		{
			for (index.x = 0; index.x < carte->lx; index += glm::ivec3(1, 0, 0))
			{
				glm::vec3 temp = glm::vec3(index) * scalaire + min + (scalaire * glm::vec3(0.5f));
				SetPos(carte, index, temp);
			}
		}
	}
}