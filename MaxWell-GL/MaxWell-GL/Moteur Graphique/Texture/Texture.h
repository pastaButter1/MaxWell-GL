#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLM/glm/glm.hpp"

struct Texture
{
	EnumGX id = -1;
	Tex::Type cible;
	Tex::FormatInterne formatInterne;
	Tex::Format format;
	Donnee::Type type;

	static void generer(Texture* const tex);
	static void detruire(Texture* const tex);
	
	static void lier(const Texture& tex);
	static void delier(const Texture& tex);
	static void attacherUnite(const Texture& tex, const ValGX unite);
	static void attacherImage(const Texture& tex, uint32_t unite, const GLenum acces);

	static void allouer1D(Texture* const tex, const ValGX niveau, const glm::ivec1 dimension, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type, const void* const pixels);
	static void allouer2D(Texture* const tex, const ValGX niveau, const glm::ivec2 dimension, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type, const void* const pixels);
	static void allouer3D(Texture* const tex, const ValGX niveau, const glm::ivec3 dimension, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type, const void* const pixels);
	static void allouerCubemap(Texture* const tex, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type);

	static void soumettre1D(Texture* const tex, const ValGX niveau, const glm::ivec1 decalage, const glm::ivec1 dimension, const void* const pixels);
	static void soumettre2D(Texture* const tex, const ValGX niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const void* const pixels);
	static void soumettre3D(Texture* const tex, const ValGX niveau, const glm::ivec3 decalage, const glm::ivec3 dimension, const void* const pixels);
	static void soumettreCubemap(Texture* const tex, const Tex::FaceCubemap face, const ValGX niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const void* const pixels);

	static void specifier(const Texture& tex, const EnumGX param, const ValGX valeur);
	static void specifierFiltre(const Texture& tex, const Tex::Filtre filtreMag, const Tex::Filtre filtreMin);
	static void specifierEtirement(const Texture& tex, const Tex::Emballage wrapS, const Tex::Emballage wrapT, const Tex::Emballage wrapR);

	static void specifierCouleurBordure(const Texture& tex, const glm::vec4 colour);
	
};