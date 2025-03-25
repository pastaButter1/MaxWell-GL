#include "Vertexbuffer.h"

void Vertexbuffer::generer(Vertexbuffer* const vbo)
{
	APPEL_GX(glGenBuffers(1, (GLuint*)&(vbo->id)));
}

void Vertexbuffer::allocation(Vertexbuffer* const vbo, const uint32_t taille)
{
	lier(*vbo);
	APPEL_GX(glBufferData(GL_ARRAY_BUFFER, taille, nullptr, GL_STATIC_DRAW));

	vbo->tailleByte = taille;
}

void Vertexbuffer::lier(const Vertexbuffer vbo)
{
	APPEL_GX(glBindBuffer(GL_ARRAY_BUFFER, vbo.id));
}

void Vertexbuffer::delier()
{
	APPEL_GX(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Vertexbuffer::transfererDonnees(const Vertexbuffer vbo, uint32_t decalage, uint32_t taille, const void* donnee)
{
	lier(vbo);
	APPEL_GX(glBufferSubData(GL_ARRAY_BUFFER, decalage, taille, donnee));
}

void Vertexbuffer::detruire(Vertexbuffer* const vbo)
{
	APPEL_GX(glDeleteBuffers(1, (GLuint*)&(vbo->id)));
}
