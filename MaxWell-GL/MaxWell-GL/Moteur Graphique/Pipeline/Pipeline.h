#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Moteur Graphique/Vertexarray/Vertexarray.h"
#include "Moteur Graphique/Framebuffer/Framebuffer.h"
#include "Moteur Graphique/Shader/Shader.h"
#include "Moteur Graphique/Model/Model.h"

struct Pipeline {
	const Framebuffer* const fbo;
	const Shader* const shader;

	glm::uvec2 tailleFenetre;

	uint32_t modeEliminationFace;

	uint32_t testProfondeur;

	uint32_t modeMelangeSRC;
	uint32_t modeMelangeDST;
	uint32_t equationMelange;

	uint32_t stencilFunc;
	uint32_t stencilRef;
	uint32_t stencilMasque;
	uint32_t stencilEchec;
	uint32_t profondeurEchec;
	uint32_t stencilProfondeurReussite;

	Pipeline(const Framebuffer* const fbo, const Shader* const shader) : fbo(fbo), shader(shader) {}

	static void init(const Pipeline& pipeline);

	static void dessiner(const Pipeline& pipeline, const Vertexarray vao, const Model& model);
};