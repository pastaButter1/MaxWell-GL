#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Moteur Graphique/Vertexarray/Vertexarray.h"
#include "Moteur Graphique/Framebuffer/Framebuffer.h"
#include "Moteur Graphique/Shader/Shader.h"
#include "Moteur Graphique/Model/Model.h"

struct Pipeline {
	const Framebuffer* fbo = nullptr;
	const Shader* shader = nullptr;

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
	
	Pipeline(const Framebuffer* const fbo, const Shader* const shader) : fbo(fbo), shader(shader) {}

	static void init(const Pipeline& pipeline);

	static void dessiner(const Pipeline& pipeline, const Vertexarray vao, const Model& model);
};