#include "Vertexarray.h"

void Vertexarray::generer(Vertexarray* const vao)
{
	APPEL_GX(glGenVertexArrays(1, (GLuint*)&vao->id));
}

void Vertexarray::ajouterAttribut(Vertexarray vao, uint32_t index, uint32_t taille, uint32_t type, bool normalizer, uint32_t pas, uint32_t decalage)
{
	lier(vao);
	APPEL_GX(glEnableVertexAttribArray(index));
	APPEL_GX(glVertexAttribPointer(index, taille, type, normalizer, pas, (void*)decalage));
}

void Vertexarray::lier(Vertexarray vao)
{
	APPEL_GX(glBindVertexArray(vao.id));
}

void Vertexarray::delier()
{
	APPEL_GX(glBindVertexArray(0));
}

void Vertexarray::detruire(Vertexarray* const vao)
{
	APPEL_GX(glDeleteVertexArrays(1, (GLuint*)&(vao->id)));
}
