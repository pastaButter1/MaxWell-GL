#pragma once

#include "Moteur Graphique/Graphique.h"

struct Indexbuffer
{
	int32_t id = -1;

	static void generer(Indexbuffer* const ibo, uint32_t taille);

	static void sendData(const Indexbuffer vbo, uint32_t decalage, uint32_t taille, void* donnee);

	static void lier(const Indexbuffer ibo);

	static void delier();

	static void detruire(Indexbuffer* const ibo);
};

