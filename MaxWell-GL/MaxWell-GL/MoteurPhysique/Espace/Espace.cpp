#include "Espace.h"

#include "Moteur Graphique/Shader/Shader.h"

void Espace::generer(Espace* const espace, const glm::ivec3 dimension)
{
	espace->dimensions = glm::ivec4(dimension, dimension.x * dimension.y * dimension.z);
	espace->longueur = dimension.x * dimension.y * dimension.z;
	espace->tableau = new glm::vec3[espace->longueur];
}

void Espace::detruire(Espace * const espace)
{
	delete[] espace->tableau;
}

void Espace::setPos(Espace * const espace, const glm::ivec3 index, const glm::vec3 valeur)
{
	espace->tableau[index.z * espace->ly * espace->lx + index.y * espace->lx + index.x] = valeur;
}

void Espace::remplir(Espace * const espace, const glm::vec3 min, const glm::vec3 max)
{
	glm::vec3 scalaire = (max - min) / glm::vec3(espace->dimensions);
	glm::ivec3 index = { 0,0,0 };

	for (index.z = 0; index.z < espace->lz; index += glm::ivec3(0, 0, 1))
	{
		for (index.y = 0; index.y < espace->ly; index += glm::ivec3(0, 1, 0))
		{
			for (index.x = 0; index.x < espace->lx; index += glm::ivec3(1, 0, 0))
			{
				glm::vec3 temp = glm::vec3(index) * scalaire + min + (scalaire * glm::vec3(0.5f));
				setPos(espace, index, temp);
			}
		}
	}
}

void Espace::GPU::initialiser(Espace* const espace, const glm::ivec3 dimension)
{
	espace->dimensions = glm::ivec4(dimension, dimension.x * dimension.y * dimension.z);
	espace->longueur = dimension.x * dimension.y * dimension.z;

	Shader::delier();

	Texture::generer(&espace->tex);
	Texture::allouer3D(&espace->tex, 0, espace->dimensions, Tex::FormatInterne::RVBA32F, Tex::Format::RVBA, Donnee::Type::VIRGULE, nullptr);
	Texture::specifierEtirement(espace->tex, Tex::Emballage::LIMITER_BORDURE, Tex::Emballage::LIMITER_BORDURE, Tex::Emballage::LIMITER_BORDURE);
	Texture::specifierCouleurBordure(espace->tex, glm::vec4(0.0f));
	Texture::specifierFiltre(espace->tex, Tex::Filtre::LINEAIRE, Tex::Filtre::LINEAIRE);
}

void Espace::GPU::soumettre(Espace* const espace)
{
	Texture::soumettre3D(&espace->tex, 0, glm::ivec3(0), espace->dimensions, espace->tableau);
}



