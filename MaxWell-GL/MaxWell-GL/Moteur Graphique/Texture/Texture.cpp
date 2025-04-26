#include "Texture.h"

void Texture::generer(Texture* const tex)
{
	APPEL_GX(glGenTextures(1, (GLuint*)&(tex->id)));
}

void Texture::detruire(Texture* const tex)
{
	APPEL_GX(glDeleteTextures(1, (GLuint*)&(tex->id)));
}

void Texture::lier(const Texture& tex)
{
	APPEL_GX(glBindTexture((EnumGX)tex.cible, tex.id));
}

{
	APPEL_GX(glBindTexture(GL_TEXTURE_2D, 0));
	APPEL_GX(glBindTexture(tex.cible, 0));
}

{
	lier(tex);
	APPEL_GX(glBindTextureUnit(unite, tex.id));
}

void Texture::attacherImage(const Texture& tex, uint32_t unite, const GLenum acces)
{
	APPEL_GX(glBindImageTexture(unite, tex.id, 0, GL_TRUE, 0, acces, tex.formatInterne));
}

void Texture::allouer1D(Texture* const tex, const ValGX niveau, const glm::ivec1 dimension, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type, const void* const pixels)
{
	tex->cible = Tex::Type::T1D;
	tex->formatInterne = formatInterne;
	tex->format = format;
	tex->type = type;
	lier(*tex);
	APPEL_GX(glTexImage1D((EnumGX)tex->cible, niveau, (EnumGX)tex->formatInterne, dimension.x, 0, (EnumGX)format, (EnumGX)type, pixels));
}

void Texture::allouer2D(Texture* const tex, const ValGX niveau, const glm::ivec2 dimension, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type, const void* const pixels)
{
	tex->cible = Tex::Type::T2D;
	tex->formatInterne = formatInterne;
	tex->format = format;
	tex->type = type;
	lier(*tex);
	APPEL_GX(glTexImage2D((EnumGX)tex->cible, niveau, (EnumGX)tex->formatInterne, dimension.x, dimension.y, 0, (EnumGX)format, (EnumGX)type, pixels));
}

void Texture::allouer3D(Texture* const tex, const ValGX niveau, const glm::ivec3 dimension, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type, const void* const pixels)
{
	tex->cible = Tex::Type::T3D;
	tex->formatInterne = formatInterne;
	tex->format = format;
	tex->type = type;
	lier(*tex);
	APPEL_GX(glTexImage3D((EnumGX)tex->cible, niveau, (EnumGX)tex->formatInterne, dimension.x, dimension.y, dimension.z, 0, (EnumGX)format, (EnumGX)type, pixels));
}

void Texture::allouerCubemap(Texture* const tex, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type type)
{
	tex->cible = Tex::Type::CUBEMAP;
	tex->formatInterne = formatInterne;
	tex->format = format;
	tex->type = type;
}

void Texture::soumettre1D(Texture* const tex, const ValGX niveau, const glm::ivec1 decalage, const glm::ivec1 dimension, const void* const pixels)
{
	lier(*tex);
	APPEL_GX(glTextureSubImage1D(tex->id, niveau, decalage.x, dimension.x, (EnumGX)tex->format, (EnumGX)tex->type, pixels));
}

void Texture::soumettre2D(Texture* const tex, const ValGX niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const void* const pixels)
{
	lier(*tex);
	APPEL_GX(glTextureSubImage2D(tex->id, niveau, decalage.x, decalage.y, dimension.x, dimension.y, (EnumGX)tex->format, (EnumGX)tex->type, pixels));
}

void Texture::soumettre3D(Texture* const tex, const ValGX niveau, const glm::ivec3 decalage, const glm::ivec3 dimension, const void* const pixels)
{
	lier(*tex);
	APPEL_GX(glTextureSubImage3D(tex->id, niveau, decalage.x, decalage.y, decalage.z, dimension.x, dimension.y, dimension.z, (EnumGX)tex->format, (EnumGX)tex->type, pixels));
}

void Texture::soumettreCubemap(Texture* const tex, const Tex::FaceCubemap face, const ValGX niveau, const glm::ivec2 decalage, const glm::ivec2 dimension, const void* const pixels)
{
	lier(*tex);
	APPEL_GX(glTexImage2D((EnumGX)face, niveau, (EnumGX)tex->formatInterne, dimension.x, dimension.y, 0, (EnumGX)tex->format, (EnumGX)tex->type, pixels));
}

void Texture::specifier(const Texture& tex, EnumGX param, ValGX valeur)
{
	lier(tex);
	APPEL_GX(glTexParameteri((EnumGX)tex.cible, param, valeur));
}

void Texture::specifierFiltre(const Texture& tex, const Tex::Filtre filtreMag, const Tex::Filtre filtreMin)
{
	lier(tex);
	APPEL_GX(glTexParameteri((EnumGX)tex.cible, GL_TEXTURE_MIN_FILTER, (EnumGX)filtreMag));
	APPEL_GX(glTexParameteri((EnumGX)tex.cible, GL_TEXTURE_MAG_FILTER, (EnumGX)filtreMin));
}

void Texture::specifierEtirement(const Texture& tex, const Tex::Emballage wrapS, const Tex::Emballage wrapT, const Tex::Emballage wrapR)
{
	lier(tex);
	APPEL_GX(glTexParameteri((EnumGX)tex.cible, GL_TEXTURE_WRAP_S, (EnumGX)wrapS));
	APPEL_GX(glTexParameteri((EnumGX)tex.cible, GL_TEXTURE_WRAP_T, (EnumGX)wrapT));
	APPEL_GX(glTexParameteri((EnumGX)tex.cible, GL_TEXTURE_WRAP_R, (EnumGX)wrapR));
}

void Texture::specifierCouleurBordure(const Texture& tex, const glm::vec4 colour)
{
	lier(tex);
	APPEL_GX(glTexParameterfv((EnumGX)tex.cible, GL_TEXTURE_BORDER_COLOR, (float*)&colour));
}
