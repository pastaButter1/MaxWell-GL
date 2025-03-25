#pragma once

#include "Moteur Graphique/Graphique.h"

struct Vertexbuffer {
	int32_t id = -1;
	uint32_t tailleByte = 0;

	static void generer(Vertexbuffer* const vbo);

	static void allocation(Vertexbuffer* const vbo, const uint32_t taille);

	static void lier(const Vertexbuffer vbo);

	static void delier();

	static void transfererDonnees(const Vertexbuffer vbo, uint32_t decalage, uint32_t taille, const void* donnee);

	static void detruire(Vertexbuffer* const vbo);
};
