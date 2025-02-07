#pragma once

#include "Moteur Graphique/Graphique.h"

struct Texture
{
	int32_t id = -1;

	static void generer(Texture* const tex, uint32_t largeur, uint32_t hauteur, const void* const donnee, uint32_t internalFormat, uint32_t format, uint32_t dataType);

	static void setFiltering(const Texture tex, uint32_t magFilter, uint32_t minFilter);

	static void setWrapping(const Texture tex, uint32_t uWrapping, uint32_t vWrapping);

	static void setBorderColour(const Texture tex, glm::vec3 colour);

	static void setBorderColour(const Texture tex, glm::vec4 colour);
	
	static void lier(const Texture tex);

	static void delier();

	static void detruire(Texture* const tex);
};