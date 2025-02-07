#pragma once

#include "Moteur Graphique/Graphique.h"

struct Cubemap {
	int32_t id;

	static void generer(Cubemap* const cubemap);

	static void lier(const Cubemap cubemap);

	static void delier();

	static void loadFace(Cubemap cubemap, uint32_t face, uint32_t largeur, uint32_t hauteur, const void* const donnee);
};