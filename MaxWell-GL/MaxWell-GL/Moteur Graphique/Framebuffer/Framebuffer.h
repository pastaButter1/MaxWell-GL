#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Moteur Graphique/Texture/Texture.h"

struct Framebuffer
{
	Texture textureAttachments[4];
	uint32_t infoAttachments = 0;
	int32_t id = -1;

	static constexpr uint32_t nbAttachmentMask = 0x00000003;
	static constexpr uint32_t masqueProfondeurStencil = 0x80000000;
	static constexpr uint32_t masqueCouleur =        0x78000000;

	static void generer(Framebuffer* const fbo);

	static void detruire(Framebuffer* const fbo);

	static void addAttachment(Framebuffer* const fbo, Texture* const tex, uint32_t largeur, uint32_t hauteur, uint32_t formatInterne, uint32_t format, uint32_t typeDonnees, uint32_t filtreMin, uint32_t filtreMag);

	static void lier(const Framebuffer& fbo);

	static void delier();
};

