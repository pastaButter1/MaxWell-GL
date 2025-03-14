#pragma once

#include "Moteur Graphique/Pipeline/Pipeline.h"
#include "Moteur Graphique/Framebuffer/Framebuffer.h"
#include "Moteur Graphique/MoteurGx/ListeIndexUnique.h"
#include "Moteur Graphique/Vertexarray/Vertexarray.h"

#include <vector>

struct MoteurGX
{
	ListeIndexUnique<uint16_t, Pipeline> listePipelines;
	ListeIndexUnique<uint16_t, Shader> listeShaders;
	ListeIndexUnique<uint16_t, Framebuffer> listeFBOs;
	ListeIndexUnique<uint16_t, Texture> listeTextures;
	ListeIndexUnique<uint16_t, Vertexarray> listeVAOs;

	static void demarerCouche(MoteurGX* const mGX);

	static void executerCouche(const MoteurGX& mGX);

	static void pousserMesh(MoteurGX* const mGX);
};