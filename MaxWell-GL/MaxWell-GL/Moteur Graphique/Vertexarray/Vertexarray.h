#pragma once

#include "Moteur Graphique/Graphique.h"

struct Vertexarray
{
	int32_t id = -1;

	static void generer(Vertexarray* const vao);

	static void ajouterAttribut(Vertexarray vao, uint32_t index, uint32_t taille, uint32_t type, bool normalizer, uint32_t pas, uint32_t decalage);

	static void lier(Vertexarray vao);

	static void delier();

	static void detruire(Vertexarray* const vao);
};

