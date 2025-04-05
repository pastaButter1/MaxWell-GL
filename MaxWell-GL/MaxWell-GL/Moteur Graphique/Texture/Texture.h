#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLM/glm/glm.hpp"

struct Texture
{
	GLuint id = -1;
	GLenum cible;
	GLenum formatInterne;

	static void generer(Texture* const tex, const GLenum cible, const GLenum formatInterne);
	static void detruire(Texture* const tex);
	
	static void lier(const Texture& tex);
	static void delier(const Texture& tex);
	static void attacherUnite(const Texture& tex, const GLuint unite);
	static void attacherImage(const Texture& tex, uint32_t unite, const GLenum acces);

	static void allouer1D(const Texture& tex, const GLint niveau, const glm::ivec1 dimension, const GLenum format, const GLenum type, const void* const pixels);
	static void allouer2D(const Texture& tex, const GLint niveau, const glm::ivec2 dimension, const GLenum format, const GLenum type, const void* const pixels);
	static void allouer3D(const Texture& tex, const GLint niveau, const glm::ivec3 dimension, const GLenum format, const GLenum type, const void* const pixels);

	static void soumettre1D(const Texture& tex, const GLint niveau, const glm::ivec1 decalage, const glm::ivec1 dimension, const GLenum format, const GLenum type, const void* const pixels);
	static void soumettre2D(const Texture& tex, const GLint niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const GLenum format, const GLenum type, const void* const pixels);
	static void soumettre3D(const Texture& tex, const GLint niveau, const glm::ivec3 decalage, const glm::ivec3 dimension, const GLenum format, const GLenum type, const void* const pixels);

	static void specifier(const Texture& tex, const GLenum param, const GLint valeur);
	/*
	* default filtreMin: GL_NEAREST_MIPMAP_LINEAR
	* valeurs possibles: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	* default filtreMag: GL_LINEAR
	* valeurs possibles: GL_NEAREST, GL_LINEAR
	*/
	static void specifierFiltre(const Texture& tex, const GLint filtreMag, const GLint filtreMin);
	/*
	* default: GL_REPEAT
	* valeurs possibles: GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT, GL_REPEAT, GL_MIRROR_CLAMP_TO_EDGE
	*/
	static void specifierEtirement(const Texture& tex, const GLint wrapS, const GLint wrapT, const GLint wrapR);
	static void specifierCouleurBordure(const Texture& tex, const glm::vec4 colour);
	
};