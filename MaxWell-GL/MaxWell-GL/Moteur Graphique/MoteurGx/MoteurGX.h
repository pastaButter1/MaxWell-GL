#pragma once

#include "Moteur Graphique/Pipeline/Pipeline.h"
#include "Moteur Graphique/Framebuffer/Framebuffer.h"
#include "Moteur Graphique/MoteurGx/ListeIndexUnique.h"
#include "Moteur Graphique/Vertexarray/Vertexarray.h"

#include <vector>

struct MoteurGX
{
	typedef uint16_t Ressource;

	ListeIndexUnique<Ressource, Pipeline> listePipelines;
	ListeIndexUnique<Ressource, Shader> listeShaders;
	ListeIndexUnique<Ressource, Framebuffer> listeFBOs;
	ListeIndexUnique<Ressource, Texture> listeTextures;
	ListeIndexUnique<Ressource, Vertexarray> listeVAOs;

	struct Couche
	{
		Ressource pipeline = 0;
		Ressource mesh = 0;
	};

	Couche coucheActive;

	MoteurGX() {}

	MoteurGX(const MoteurGX&) = delete;
	MoteurGX& operator=(const MoteurGX&) = delete;

	static Pipeline& creerPipeline(MoteurGX* const mGX, Ressource* const res);

	static Shader& creeerShader(MoteurGX* const mGX, Ressource* const res);

	static Framebuffer& creerFramebuffer(MoteurGX* const mGX, Ressource* const res);

	static Texture& creerTexture(MoteurGX* const mGX, Ressource* const res);

	static Vertexarray& creerVertexarray(MoteurGX* const mGX, Ressource* const res);

	static void demarerCouche(const MoteurGX&  mGX, const Ressource pipeline);

	static void executerCouche(const MoteurGX& mGX);

	static void pousserMesh(MoteurGX* const mGX);
};