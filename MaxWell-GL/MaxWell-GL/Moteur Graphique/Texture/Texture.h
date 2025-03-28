#pragma once

#include "Moteur Graphique/Graphique.h"
#include "Lib/GLM/glm/glm.hpp"

struct Texture
{
	GLuint id = -1;
	GLenum cible;
	GLenum formatInterne;

	static void generer(Texture* const tex);
	static void detruire(Texture* const tex);
	
	static void lier(const Texture& tex);
	static void delier();
	static void attacher(const Texture& tex, const GLuint unite);

	static void allouer1D(Texture* const tex, const GLint niveau, const glm::ivec1 dimension, const GLenum formatInterne, const GLenum format, const GLenum type, const void* const pixels);
	static void allouer2D(Texture* const tex, const GLint niveau, const glm::ivec2 dimension, const GLenum formatInterne, const GLenum format, const GLenum type, const void* const pixels);
	static void allouer3D(Texture* const tex, const GLint niveau, const glm::ivec3 dimension, const GLenum formatInterne, const GLenum format, const GLenum type, const void* const pixels);

	static void soumettre1D(Texture* const tex, const GLint niveau, const glm::ivec1 decalage, const glm::ivec1 dimension, const GLenum format, const GLenum type, const void* const pixels);
	static void soumettre2D(Texture* const tex, const GLint niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const GLenum format, const GLenum type, const void* const pixels);
	static void soumettre3D(Texture* const tex, const GLint niveau, const glm::ivec3 decalage, const glm::ivec3 dimension, const GLenum format, const GLenum type, const void* const pixels);

	static void specifier(const Texture& tex, const GLenum param, const GLint valeur);
	static void specifierFiltre(const Texture& tex, const GLint filtreMag, const GLint filtreMin);
	static void specifierEtirement(const Texture& tex, const GLint wrapS, const GLint wrapT, const GLint wrapR);
	static void specifierCouleurBordure(const Texture& tex, const glm::vec4 colour);
	
};