#pragma once

#include "Moteur Graphique/Graphique.h"

#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"

struct Vertexarray
{
	int32_t id = -1;
	uint32_t nbTriangles = 0;

	static void generer(Vertexarray* const vao);

	static void ajouterAttribut(Vertexarray vao, Vertexbuffer vbo, EnumGX index, uint32_t taille, EnumGX type, bool normalizer, uint32_t pas, uint32_t decalage);

	static void lier(Vertexarray vao);

	static void delier();

	static void detruire(Vertexarray* const vao);
};

