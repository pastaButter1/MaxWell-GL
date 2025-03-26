#include "Mesh.h"

void mgx::Mesh::creer(Mesh* const mesh, MoteurGX* const mGX)
{
	Vertexarray& vao = MoteurGX::creerVertexarray(mGX, & mesh->vao);
	Vertexbuffer& vbo = MoteurGX::creerVertexbuffer(mGX, &mesh->vbo);

	vao.nbTriangles = 0;
	vbo.tailleByte = 0;
}

void mgx::Mesh::chargerModel(Mesh* const mesh, MoteurGX* const mGX, const uint32_t nbTriangles, const uint32_t tailleByte, const void* const donnees)
{
	Vertexarray& vao = MoteurGX::retVertexarray(*mGX, mesh->vao);
	Vertexbuffer& vbo = MoteurGX::retVertexbuffer(*mGX, mesh->vbo);

	Vertexbuffer::allocation(&vbo, tailleByte);
	Vertexbuffer::transfererDonnees(vbo, 0, tailleByte, donnees);

	vao.nbTriangles = nbTriangles;
}
