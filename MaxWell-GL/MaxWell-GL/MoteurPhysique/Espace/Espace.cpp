#include "Espace.h"

void Espace::Initialiser(Espace* const espace, const glm::ivec3 dimension)
{
	espace->dimensions = glm::ivec4(dimension, dimension.x * dimension.y * dimension.z);
	espace->longueur = dimension.x * dimension.y * dimension.z;
	espace->tableau = new glm::vec3[espace->longueur];
}

void Espace::Detruire(Espace * const espace)
{
	delete[] carte->tableau;
}

void Espace::SetPos(Espace * const espace, const glm::ivec3 index, const glm::vec3 valeur)
{
	carte->tableau[index.z * carte->ly * carte->lx + index.y * carte->lx + index.x] = valeur;
}

void Espace::Remplir(Espace * const espace, const glm::vec3 min, const glm::vec3 max)
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

void Espace::gpuInitialiser(Espace* const espace, const glm::ivec3 dimension)
{
	espace->dimensions = glm::ivec4(dimension, dimension.x * dimension.y * dimension.z);
	espace->longueur = dimension.x * dimension.y * dimension.z;

	APPEL_GX(glUseProgram(0));
	Texture::generer(&espace->tex, GL_TEXTURE_3D, GL_RGBA32F);
	Texture::specifierEtirement(espace->tex, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	Texture::specifierCouleurBordure(espace->tex, glm::vec4(0.0f));
	Texture::specifierFiltre(espace->tex, GL_LINEAR, GL_LINEAR);
	Texture::allouer3D(espace->tex, 0, espace->dimensions, GL_RGB, GL_FLOAT, nullptr);
}

void Espace::gpuSoumettre(const Espace& espace)
{
	Texture::soumettre3D(espace.tex, 0, glm::ivec3(0), espace.dimensions, GL_RGB, GL_FLOAT, espace.tableau);
}



