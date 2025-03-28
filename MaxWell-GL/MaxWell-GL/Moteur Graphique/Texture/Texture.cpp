#include "Texture.h"

void Texture::generer(Texture* const tex, const GLenum cible, const GLenum formatInterne)
{
	tex->cible = cible;
	tex->formatInterne = formatInterne;

	APPEL_GX(glGenTextures(1, (GLuint*)&(tex->id)));
	
	lier(*tex);

	/*
	if (formatInterne == TEX_PROFONDEUR_STENCIL)
	{
		dataType = GL_UNSIGNED_INT_24_8;
	}
	*/
}

void Texture::detruire(Texture* const tex)
{
	APPEL_GX(glDeleteTextures(1, (GLuint*)&(tex->id)));
}

void Texture::lier(const Texture& tex)
{
	APPEL_GX(glBindTexture(GL_TEXTURE_2D, tex.id));
}

void Texture::delier()
{
	APPEL_GX(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::attacherUnite(const Texture& tex, const GLuint unite)
{
	APPEL_GX(glBindTextureUnit(unite, tex.id));
}

void Texture::attacherImage(const Texture& tex, uint32_t unite, const GLenum acces)
{
	glBindImageTexture(unite, tex.id, 0, GL_TRUE, 0, acces, tex.formatInterne);
}

void Texture::allouer1D(const Texture& tex, const GLint niveau, const glm::ivec1 dimension, const GLenum format, const GLenum type, const void* const pixels)
{
	APPEL_GX(glTexImage1D(tex.cible, niveau, tex.formatInterne, dimension.x, 0, format, type, pixels));
}

void Texture::allouer2D(const Texture& tex, const GLint niveau, const glm::ivec2 dimension, const GLenum format, const GLenum type, const void* const pixels)
{
	APPEL_GX(glTexImage2D(tex.cible, niveau, tex.formatInterne, dimension.x, dimension.y, 0, format, type, pixels));
}

void Texture::allouer3D(const Texture& tex, const GLint niveau, const glm::ivec3 dimension, const GLenum format, const GLenum type, const void* const pixels)
{
	APPEL_GX(glTexImage3D(tex.cible, niveau, tex.formatInterne, dimension.x, dimension.y, dimension.z, 0, format, type, pixels));
}

void Texture::soumettre1D(const Texture& tex, const GLint niveau, const glm::ivec1 decalage, const glm::ivec1 dimension, const GLenum format, const GLenum type, const void* const pixels)
{
	APPEL_GX(glTextureSubImage1D(tex.id, niveau, decalage.x, dimension.x, format, type, pixels));
}

void Texture::soumettre2D(const Texture& tex, const GLint niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const GLenum format, const GLenum type, const void* const pixels)
{
	APPEL_GX(glTextureSubImage2D(tex.id, niveau, decalage.x, decalage.y, dimension.x, dimension.y, format, type, pixels));
}

void Texture::soumettre3D(const Texture& tex, const GLint niveau, const glm::ivec3 decalage, const glm::ivec3 dimension, const GLenum format, const GLenum type, const void* const pixels)
{
	APPEL_GX(glTextureSubImage3D(tex.id, niveau, decalage.x, decalage.y, decalage.z, dimension.x, dimension.y, dimension.z, format, type, pixels));
}

void Texture::specifier(const Texture& tex, GLenum param, GLint valeur)
{
	APPEL_GX(glTexParameteri(tex.cible, param, valeur));
}

void Texture::specifierFiltre(const Texture& tex, const GLint filtreMag, const GLint filtreMin)
{
	APPEL_GX(glTexParameteri(tex.cible, GL_TEXTURE_MIN_FILTER, filtreMag));
	APPEL_GX(glTexParameteri(tex.cible, GL_TEXTURE_MAG_FILTER, filtreMin));
}

void Texture::specifierEtirement(const Texture& tex, const GLint wrapS, const GLint wrapT, const GLint wrapR)
{
	APPEL_GX(glTexParameteri(tex.cible, GL_TEXTURE_WRAP_S, wrapS));
	APPEL_GX(glTexParameteri(tex.cible, GL_TEXTURE_WRAP_T, wrapT));
	APPEL_GX(glTexParameteri(tex.cible, GL_TEXTURE_WRAP_R, wrapR));
}

void Texture::specifierCouleurBordure(const Texture& tex, const glm::vec4 colour)
{
	APPEL_GX(glTexParameterfv(tex.cible, GL_TEXTURE_BORDER_COLOR, (float*)&colour));
}
