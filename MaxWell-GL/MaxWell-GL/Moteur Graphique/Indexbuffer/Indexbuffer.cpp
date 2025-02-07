#include "Indexbuffer.h"

void Indexbuffer::generer(Indexbuffer* const vbo, uint32_t taille)
{
	APPEL_GX(glGenBuffers(1, (GLuint*)&(vbo->id)));
	lier(*vbo);
	APPEL_GX(glBufferData(GL_ELEMENT_ARRAY_BUFFER, taille, nullptr, GL_STATIC_DRAW));
}

void Indexbuffer::lier(const Indexbuffer vbo)
{
	APPEL_GX(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.id));
}

void Indexbuffer::delier()
{
	APPEL_GX(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Indexbuffer::detruire(Indexbuffer* const ibo)
{
	APPEL_GX(glDeleteBuffers(1, (GLuint*)&(ibo->id)));
}

void Indexbuffer::sendData(const Indexbuffer vbo, uint32_t decalage, uint32_t taille, void* donnee)
{
	lier(vbo);
	APPEL_GX(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, decalage, taille, donnee));
}
