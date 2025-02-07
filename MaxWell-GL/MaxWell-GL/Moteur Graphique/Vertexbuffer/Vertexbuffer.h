#pragma once

#include "Moteur Graphique/Graphique.h"

struct Vertexbuffer {
	int32_t id = -1;

	static void generer(Vertexbuffer* const vbo, uint32_t taille);

	static void lier(const Vertexbuffer vbo);

	static void delier();

	static void transfererDonnees(const Vertexbuffer vbo, uint32_t decalage, uint32_t taille, void* donnee);

	static void detruire(Vertexbuffer* const vbo);
};
