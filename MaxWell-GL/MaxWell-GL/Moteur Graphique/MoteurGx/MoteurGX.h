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

	/** Contient les param�tres n�cessaire � un drawcall */
	struct Pipeline {
		Ressource fbo;
		Ressource shader;

		Operation::Dessin modeDessin;

		glm::uvec2 tailleFenetre;

		Operation::Culling modeCulling;

		Operation::Profondeur testProfondeur;

		Operation::FuncMelange modeMelangeSRC;
		Operation::FuncMelange modeMelangeDST;
		Operation::Melange equationMelange;

		Operation::Stencil stencilFunc;
		EnumGX stencilRef;
		EnumGX stencilMasque;
		Operation::TestSP stencilEchec;
		Operation::TestSP profondeurEchec;
		Operation::TestSP stencilProfondeurReussit;
		bool nettoyerCible;

		Pipeline() {}

		static void init(const Pipeline& pipeline, const Framebuffer& fbo, const Shader& shader);

		/** Transmet au GPU le drawcall */
		static void dessiner(const Pipeline& pipeline, const Vertexarray vao);
	
		/** Initialise avec des param�tres standard (blend, depth, ...) */
		static constexpr void renduStandard(Pipeline* const pipelinePtr, const bool nettoyerCible)
		{
			using namespace Operation;

			Pipeline& pipeline = *pipelinePtr;
			pipeline.modeDessin = Dessin::TRIANGLES_PLEINS;
			pipeline.equationMelange = Melange::ADDITION;
			pipeline.modeMelangeSRC = FuncMelange::ALPHA_SRC;
			pipeline.modeMelangeDST = FuncMelange::UN_MOINS_ALPHA_SRC;
			pipeline.testProfondeur = Profondeur::PLUS_PETIT;
			pipeline.modeCulling = Culling::ARRIERE;
			pipeline.stencilFunc = Stencil::TOUJOURS;
			pipeline.stencilMasque = 0xFF;
			pipeline.stencilRef = 0xFF;
			pipeline.stencilEchec = TestSP::GARDER;
			pipeline.profondeurEchec = TestSP::GARDER;
			pipeline.stencilProfondeurReussit = TestSP::GARDER;
			pipeline.nettoyerCible = nettoyerCible;
		}
	};
}

/**
* 
* Structure permettant facilement d'effectuer des t�ches graphiques sur le GPU
* 
* Note : Le ressource ne sont que des index �tant utilis�s par les tampons pour distinguer les objets qu'ils contiennent
* 
*/
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

	/** Initialisation des tampons de ressources et cr�ation des ressources de base comme une cible graphique (FBO) */
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

	/** 
	* Initialise le ressource et les param�tres n�cessaires � un programme (shader) particulier
	* 
	* Les constantes "uniform" d'un programme doivent �tre envoyer manuellement � l'aide des fonctions dans la classe shader
	*/
	static const Shader& demarerProgramme(const MoteurGX&  mGX, const Ressource pipeline);

	/** Envoie une requ�te au GPU pour �x�cuter une t�che (Drawcall) avec un tampon de triangles (vaoIU) */
	static void executerProgramme(const MoteurGX& mGX, const Ressource pipelineIU, const Ressource vaoIU);

	/** Ne fait rien, � retirer */
	static void pousserMesh(MoteurGX* const mGX);

	/** Copie l'image produite avec les divers programmes sur le FBO de l'�cran */
	static void copierRenduBackbuffer(const MoteurGX& mGX, const glm::uvec2 dimensionBackbuffer);
};