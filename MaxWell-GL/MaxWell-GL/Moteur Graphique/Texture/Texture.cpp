#include "Texture.h"

void Texture::generer(Texture* const tex, uint32_t largeur, uint32_t hauteur, const void* const donnee, uint32_t internalFormat, uint32_t format, uint32_t dataType)
{
	APPEL_GX(glGenTextures(1, (GLuint*)&(tex->id)));
	lier(*tex);

	if (internalFormat == TEX_PROFONDEUR_STENCIL)
	{
		dataType = GL_UNSIGNED_INT_24_8;
	}
	APPEL_GX(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, largeur, hauteur, 0, format, dataType, donnee));
}

void Texture::specifierFiltre(const Texture tex, uint32_t magFilter, uint32_t minFilter)
{
	lier(tex);
	APPEL_GX(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
	APPEL_GX(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
}

void Texture::specifierEtirement(const Texture tex, uint32_t uWrapping, uint32_t vWrapping)
{
	APPEL_GX(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uWrapping));
	APPEL_GX(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrapping));
}

void Texture::specifierCouleurBordure(const Texture tex, glm::vec3 colour)
{
	APPEL_GX(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&colour));
}

void Texture::specifierCouleurBordure(const Texture tex, glm::vec4 colour)
{
	APPEL_GX(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&colour));
}

void Texture::lier(const Texture tex)
{
	APPEL_GX(glBindTexture(GL_TEXTURE_2D, tex.id));
}

void Texture::delier()
{
	APPEL_GX(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::detruire(Texture* const tex)
{
	APPEL_GX(glDeleteTextures(1, (GLuint*)&(tex->id)));
}
