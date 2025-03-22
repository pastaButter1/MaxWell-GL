#include "MoteurGX.h"

#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"

using namespace mgx;

void mgx::Pipeline::init(const Pipeline& pipeline, const Framebuffer& fbo, const Shader& shader)
{
	//Framebuffer::lier(fbo);
	Shader::lier(shader);

	APPEL_GX(glViewport(0, 0, pipeline.tailleFenetre.x, pipeline.tailleFenetre.y));
	APPEL_GX(glScissor(0, 0, pipeline.tailleFenetre.x, pipeline.tailleFenetre.y));
	APPEL_GX(glCullFace(pipeline.modeEliminationFace));
	APPEL_GX(glDepthFunc(pipeline.testProfondeur));
	APPEL_GX(glBlendFunc(pipeline.modeMelangeSRC, pipeline.modeMelangeDST));
	APPEL_GX(glBlendEquation(pipeline.equationMelange));

	APPEL_GX(glStencilFunc(pipeline.stencilFunc, pipeline.stencilRef, pipeline.stencilMasque));
	APPEL_GX(glStencilOp(pipeline.stencilEchec, pipeline.profondeurEchec, pipeline.stencilProfondeurReussite));

	// Definir les cibles du dessinage

	/*const uint32_t masqueAttachements = Framebuffer::masqueCouleur;
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

	APPEL_GX(glDrawBuffers(nbAttachements, destinations));*/
}

void mgx::Pipeline::dessiner(const Pipeline& pipeline, const Vertexarray vao)
{
	Vertexarray::lier(vao);
	APPEL_GX(glDrawArrays(pipeline.modeDessin, 0, vao.nbTriangles * 3));
}

void MoteurGX::init(MoteurGX* const mGX)
{
	Ressource res;
	Framebuffer& fbo = MoteurGX::creerFramebuffer(mGX, &res);
	Framebuffer::addAttachment(&fbo, 800, 600, TEX_INFORMAT_RVBA, TEX_FORMAT_RVBA, GL_UNSIGNED_BYTE, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);
}

Pipeline& MoteurGX::creerPipeline(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listePipelines.ajouter();
	*res = index;

	return mGX->listePipelines.rechercherIndexUnique(index);
}

Shader& MoteurGX::creerShader(MoteurGX* const mGX, Ressource* const res)
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

Pipeline& MoteurGX::retPipeline(const MoteurGX& mGX, const Ressource res)
{
	return *(Pipeline*)&mGX.listePipelines.rechercherIndexUnique(res);
}

Shader& MoteurGX::retShader(const MoteurGX& mGX, const Ressource res)
{
	return *(Shader*)&mGX.listeShaders.rechercherIndexUnique(res);
}

Framebuffer& MoteurGX::retFBO(const MoteurGX& mGX, const Ressource res)
{
	return *(Framebuffer*)&mGX.listeFBOs.rechercherIndexUnique(res);
}

Texture& MoteurGX::retTexture(const MoteurGX& mGX, const Ressource res)
{
	return *(Texture*)&mGX.listeTextures.rechercherIndexUnique(res);
}

Vertexarray& MoteurGX::retVertexarray(const MoteurGX& mGX, const Ressource res)
{
	return *(Vertexarray*)&mGX.listeVAOs.rechercherIndexUnique(res);
}

void MoteurGX::demarerCouche(const MoteurGX& mGX, const Ressource IndexPipeline)
{
	const Pipeline& pipeline = mGX.listePipelines.rechercherIndexUnique(IndexPipeline);
	const Framebuffer& fbo = MoteurGX::retFBO(mGX, pipeline.fbo);
	const Shader& shader = MoteurGX::retShader(mGX, pipeline.shader);

	Pipeline::init(pipeline, fbo, shader);
}

void MoteurGX::executerCouche(const MoteurGX& mGX)
{
	APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	APPEL_GX(glClearColor(1, 0, 1, 1));
	APPEL_GX(glClear(GL_COLOR_BUFFER_BIT));

	const Couche& couche = mGX.coucheActive;

	const Pipeline& pipeline = mGX.listePipelines.rechercherIndexUnique(0);

	const Vertexarray& vao = mGX.listeVAOs.rechercherIndexUnique(0);

	//Vertexbuffer vbo;
	//vbo.id = 1;
	//
	//Vertexbuffer::lier(vbo);

	Pipeline::dessiner(pipeline, vao);

	//Framebuffer::lier(mGX.listeFBOs.rechercherIndexUnique(0));
}

void MoteurGX::pousserMesh(MoteurGX* const mGX)
{
}