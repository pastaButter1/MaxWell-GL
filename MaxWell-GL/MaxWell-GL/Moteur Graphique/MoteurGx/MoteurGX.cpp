#include "MoteurGX.h"

Pipeline& MoteurGX::creerPipeline(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listePipelines.ajouter();
	*res = index;

	return mGX->listePipelines.rechercherIndexUnique(index);
}

Shader& MoteurGX::creeerShader(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listeShaders.ajouter();
	*res = index;

	Shader& shader = mGX->listeShaders.rechercherIndexUnique(index);

	Shader::generer(&shader);

	return shader;
}

Framebuffer& MoteurGX::creerFramebuffer(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listeFBOs.ajouter();
	*res = index;

	Framebuffer& fbo = mGX->listeFBOs.rechercherIndexUnique(index);

	Framebuffer::generer(&fbo);

	return fbo;
}

Texture& MoteurGX::creerTexture(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listeTextures.ajouter();
	*res = index;

	return mGX->listeTextures.rechercherIndexUnique(index);
}

Vertexarray& MoteurGX::creerVertexarray(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listeVAOs.ajouter();
	*res = index;

	Vertexarray& vao = mGX->listeVAOs.rechercherIndexUnique(index);

	Vertexarray::generer(&vao);

	return vao;
}

void MoteurGX::demarerCouche(const MoteurGX& mGX, const Ressource IndexPipeline)
{
	const Pipeline& pipeline = mGX.listePipelines.rechercherIndexUnique(IndexPipeline);
	Pipeline::init(pipeline);

	const Framebuffer& fbo = *pipeline.fbo;
	const uint32_t masqueAttachements = Framebuffer::masqueCouleur;
	const uint32_t listeAttachement = fbo.infoAttachments & (masqueAttachements);

	EnumGX destinations[Framebuffer::nbAttachmentMask];
	uint32_t nbAttachements = 0;
	for (uint32_t masque = 0x80000000; nbAttachements < __popcnt(masqueAttachements); masque >>= 1)
	{
		if ((listeAttachement & masque) != 0)
		{
			destinations[nbAttachements] = GL_COLOR_ATTACHMENT0 + nbAttachements;
			nbAttachements++;
		}
	}

	APPEL_GX(glDrawBuffers(nbAttachements, destinations));
}

void MoteurGX::executerCouche(const MoteurGX& mGX)
{
	const Couche& couche = mGX.coucheActive;

	const Pipeline& pipeline = mGX.listePipelines.rechercherIndexUnique(couche.pipeline);

	const Vertexarray& vao = mGX.listeVAOs.rechercherIndexUnique(couche.mesh);

	Pipeline::dessiner(pipeline, vao);
}