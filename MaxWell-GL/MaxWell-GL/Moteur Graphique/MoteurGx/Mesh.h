#pragma once

#include "Moteur Graphique/MoteurGx/MoteurGX.h"
#include "Moteur Graphique/Model/Model.h"

namespace mgx
{
	struct Mesh
	{
		Ressource vao;
		Ressource vbo;

		/** Cr�ation des ressources pour le mesh sur le GPU */
		static void creer(Mesh* const mesh, MoteurGX* const mGX);

		/** Transfert les donn�es du model vers la m�moire du GPU*/
		template<typename T>
		static void chargerModel(Mesh* const mesh, MoteurGX* const mGX, const uint32_t nbPoints, const void* const donnees)
		{
			chargerModel(mesh, mGX, nbPoints / 3, nbPoints * sizeof(T), donnees);
		}

		/** Transfert les donn�es du model vers la m�moire du GPU et lie des attributs pour le vertex array n�cessaires � un Vertex*/
		template<>
		static void chargerModel<Vertex>(Mesh* const mesh, MoteurGX* const mGX, const uint32_t nbPoints, const void* const donnees)
		{
			chargerModel(mesh, mGX, nbPoints / 3, nbPoints * sizeof(Vertex), donnees);

			Vertexarray& vao = MoteurGX::retVertexarray(*mGX, mesh->vao);
			Vertexbuffer& vbo = MoteurGX::retVertexbuffer(*mGX, mesh->vbo);

			Vertexarray::ajouterAttribut(vao, vbo, 0, 3, Donnee::Type::VIRGULE, (EnumGX)Donnee::Binaire::FAUX, sizeof(Vertex), 0);
			Vertexarray::ajouterAttribut(vao, vbo, 1, 2, Donnee::Type::VIRGULE, (EnumGX)Donnee::Binaire::FAUX, sizeof(Vertex), offsetof(Vertex, Vertex::uv));
			Vertexarray::ajouterAttribut(vao, vbo, 2, 3, Donnee::Type::VIRGULE, (EnumGX)Donnee::Binaire::FAUX, sizeof(Vertex), offsetof(Vertex, Vertex::norm));
		}

		/** Transfert les donn�es du model vers la m�moire du GPU */
		static void chargerModel(Mesh* const mesh, MoteurGX* const mGX, const uint32_t nbTriangles, const uint32_t tailleByte, const void* const donnees);
	};
}