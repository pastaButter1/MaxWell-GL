#pragma once

#include "Moteur Graphique/MoteurGx/MoteurGXDef.h"

#include "Moteur Graphique/Framebuffer/Framebuffer.h"
#include "Moteur Graphique/MoteurGx/ListeIndexUnique.h"
#include "Moteur Graphique/Vertexarray/Vertexarray.h"
#include "Moteur Graphique/Shader/Shader.h"
#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"

#include <vector>

namespace mgx
{
	struct Pipeline {
		Ressource fbo;
		Ressource shader;

		EnumGX modeDessin;

		glm::uvec2 tailleFenetre;

		EnumGX modeEliminationFace;

		EnumGX testProfondeur;

		EnumGX modeMelangeSRC;
		EnumGX modeMelangeDST;
		EnumGX equationMelange;

		EnumGX stencilFunc;
		EnumGX stencilRef;
		EnumGX stencilMasque;
		EnumGX stencilEchec;
		EnumGX profondeurEchec;
		EnumGX stencilProfondeurReussite;

		Pipeline() {}

		static void init(const Pipeline& pipeline, const Framebuffer& fbo, const Shader& shader);

		static void dessiner(const Pipeline& pipeline, const Vertexarray vao);

		static constexpr void renduStandard(Pipeline* const pipelinePtr);
	};
}

struct MoteurGX
{
	using Ressource = mgx::Ressource;
	using Pipeline = mgx::Pipeline;

	struct Couche
	{
		Ressource pipeline = 0;
		Ressource mesh = 0;
	};

	ListeIndexUnique<Ressource, Pipeline> listePipelines;
	ListeIndexUnique<Ressource, Shader> listeShaders;
	ListeIndexUnique<Ressource, Framebuffer> listeFBOs;
	ListeIndexUnique<Ressource, Texture> listeTextures;
	ListeIndexUnique<Ressource, Vertexarray> listeVAOs;
	ListeIndexUnique<Ressource, Vertexbuffer> listeVBOs;

	Couche coucheActive;

	MoteurGX() {}

	MoteurGX(const MoteurGX&) = delete;
	MoteurGX& operator=(const MoteurGX&) = delete;

	static void init(MoteurGX* const mGX);

	static Pipeline& creerPipeline(MoteurGX* const mGX, Ressource* const res);

	static Shader& creerShader(MoteurGX* const mGX, Ressource* const res);

	static Framebuffer& creerFramebuffer(MoteurGX* const mGX, Ressource* const res);

	static Texture& creerTexture(MoteurGX* const mGX, Ressource* const res);

	static Vertexarray& creerVertexarray(MoteurGX* const mGX, Ressource* const res);

	static Vertexbuffer& creerVertexbuffer(MoteurGX* const mGX, Ressource* const res);

	static Pipeline& retPipeline(const MoteurGX& mGX, const Ressource res);

	static Shader& retShader(const MoteurGX& mGX, const Ressource res);

	static Framebuffer& retFBO(const MoteurGX& mGX, const Ressource res);

	static Texture& retTexture(const MoteurGX& mGX, const Ressource res);

	static Vertexarray& retVertexarray(const MoteurGX& mGX, const Ressource res);

	static Vertexbuffer& retVertexbuffer(const MoteurGX& mGX, const Ressource res);

	static const Shader& demarerCouche(const MoteurGX&  mGX, const Ressource pipeline);

	static void executerCouche(const MoteurGX& mGX);

	static void pousserMesh(MoteurGX* const mGX);

	static void copierRenduBackbuffer(const MoteurGX& mGX, const glm::uvec2 dimensionBackbuffer);
};