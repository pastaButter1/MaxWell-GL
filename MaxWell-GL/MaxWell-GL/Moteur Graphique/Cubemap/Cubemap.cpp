#include "Cubemap.h"

void Cubemap::generer(Cubemap* const cubemap)
{
	APPEL_GX(glGenTextures(1, (uint32_t*)&cubemap->id));

	lier(*cubemap);

	APPEL_GX(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	APPEL_GX(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	APPEL_GX(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	APPEL_GX(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	APPEL_GX(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

void Cubemap::lier(const Cubemap cubemap)
{
	APPEL_GX(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.id));
}

void Cubemap::delier()
{
	APPEL_GX(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void Cubemap::loadFace(Cubemap cubemap, uint32_t face, uint32_t largeur, uint32_t hauteur, const void* const donnee)
{
	lier(cubemap);

	APPEL_GX(glTexImage2D(face, 0, GL_RGB, largeur, hauteur, 0, GL_RGB, GL_UNSIGNED_BYTE, donnee));
}
