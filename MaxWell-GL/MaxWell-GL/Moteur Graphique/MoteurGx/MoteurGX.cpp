#include "MoteurGX.h"

#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"

using namespace mgx;

void mgx::Pipeline::init(const Pipeline& pipeline, const Framebuffer& fbo, const Shader& shader)
{
	Framebuffer::lier(fbo);
	Shader::lier(shader);

	APPEL_GX(glViewport(0, 0, pipeline.tailleFenetre.x, pipeline.tailleFenetre.y));
	APPEL_GX(glScissor(0, 0, pipeline.tailleFenetre.x, pipeline.tailleFenetre.y));
	APPEL_GX(glCullFace(pipeline.modeEliminationFace));
	APPEL_GX(glDepthFunc(pipeline.testProfondeur));
	APPEL_GX(glBlendFunc(pipeline.modeMelangeSRC, pipeline.modeMelangeDST));
	APPEL_GX(glBlendEquation(pipeline.equationMelange));

	APPEL_GX(glStencilFunc(pipeline.stencilFunc, pipeline.stencilRef, pipeline.stencilMasque));
	APPEL_GX(glStencilOp(pipeline.stencilEchec, pipeline.profondeurEchec, pipeline.stencilProfondeurReussite));

	APPEL_GX(glClearColor(0.1, 0.1, 0.3, 1));
	APPEL_GX(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Definir les cibles du dessinage

	const uint32_t masqueAttachements = Framebuffer::masqueCouleur;
	const uint32_t listeAttachement = fbo.infoAttachments & (masqueAttachements);

	EnumGX destinations[Framebuffer::nbAttachmentMask];
	uint32_t nbAttachements = 0;
	for (uint32_t masque = 0x80000000, i = 0; i < __popcnt(masqueAttachements); masque >>= 1, i++)
	{
		uint32_t a = listeAttachement & masque;
		if ((listeAttachement & masque) != 0)
		{
			destinations[nbAttachements] = GL_COLOR_ATTACHMENT0 + nbAttachements;
			nbAttachements++;
		}
	}

	APPEL_GX(glDrawBuffers(nbAttachements, destinations));
}

void mgx::Pipeline::dessiner(const Pipeline& pipeline, const Vertexarray vao)
{
	Vertexarray::lier(vao);
	APPEL_GX(glDrawArrays(pipeline.modeDessin, 0, vao.nbTriangles * 3));
}

constexpr void mgx::Pipeline::renduStandard(Pipeline* const pipelinePtr)
{
	Pipeline& pipeline = *pipelinePtr;
	pipeline.modeDessin = GL_TRIANGLES;
	pipeline.equationMelange = GL_FUNC_ADD;
	pipeline.modeMelangeSRC = GL_SRC_ALPHA;
	pipeline.modeMelangeDST = GL_ONE_MINUS_SRC_ALPHA;
	pipeline.testProfondeur = GL_LESS;
	pipeline.modeEliminationFace = GL_BACK;
	pipeline.stencilFunc = FUNC_TOUJOURS;
	pipeline.stencilMasque = 0xFF;
	pipeline.stencilRef = 0xFF;
	pipeline.stencilEchec = STENCIL_FUNC_GARDER;
	pipeline.profondeurEchec = STENCIL_FUNC_GARDER;
	pipeline.stencilProfondeurReussite = STENCIL_FUNC_GARDER;
}

void MoteurGX::init(MoteurGX* const mGX)
{
	APPEL_GX(glEnable(GL_BLEND));
	APPEL_GX(glEnable(GL_DEPTH_TEST));
	APPEL_GX(glEnable(GL_STENCIL_TEST));
	//APPEL_GX(glEnable(GL_CULL_FACE));

	Ressource fboIU, texCoulIU, texProfIU;
	Framebuffer& fbo = MoteurGX::creerFramebuffer(mGX, &fboIU);
	MoteurGX::creerTexture(mGX, &texCoulIU);
	Texture& texProfondeur = MoteurGX::creerTexture(mGX, &texProfIU);
	Texture& texCouleur = MoteurGX::retTexture(*mGX, texCoulIU);
	Framebuffer::addAttachment(&fbo, &texCouleur, 800, 600, TEX_INFORMAT_RVBA, TEX_FORMAT_RVBA, GL_UNSIGNED_BYTE, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);
	Framebuffer::addAttachment(&fbo, &texProfondeur, 800, 600, TEX_INFORMAT_COMPOSANT_PROFONDEUR, TEX_FORMAT_COMPOSANT_PROFONDEUR, GL_UNSIGNED_INT, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);
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

	Texture& tex = mGX->listeTextures.rechercherIndexUnique(index);
	Texture::generer(&tex);

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

Vertexbuffer& MoteurGX::creerVertexbuffer(MoteurGX* const mGX, Ressource* const res)
{
	Ressource index = mGX->listeVBOs.ajouter();
	*res = index;

	Vertexbuffer& vbo = mGX->listeVBOs.rechercherIndexUnique(index);

	Vertexbuffer::generer(&vbo);

	return vbo;
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

Vertexbuffer& MoteurGX::retVertexbuffer(const MoteurGX& mGX, const Ressource res)
{
	return *(Vertexbuffer*)&mGX.listeVBOs.rechercherIndexUnique(res);
}

const Shader& MoteurGX::demarerCouche(const MoteurGX& mGX, const Ressource IndexPipeline)
{
	const Pipeline& pipeline = mGX.listePipelines.rechercherIndexUnique(IndexPipeline);
	const Framebuffer& fbo = MoteurGX::retFBO(mGX, pipeline.fbo);
	const Shader& shader = MoteurGX::retShader(mGX, pipeline.shader);

	Pipeline::init(pipeline, fbo, shader);

	return shader;
}

void MoteurGX::executerCouche(const MoteurGX& mGX)
{
	const Couche& couche = mGX.coucheActive;

	const Pipeline& pipeline = MoteurGX::retPipeline(mGX, 0);

	const Vertexarray& vao = MoteurGX::retVertexarray(mGX, 0);

	//APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, 0));

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

void MoteurGX::copierRenduBackbuffer(const MoteurGX& mGX, const glm::uvec2 dimensionBackbuffer)
{
	Framebuffer& fboRendu = retFBO(mGX, 0);

	APPEL_GX(glBindFramebuffer(GL_READ_FRAMEBUFFER, fboRendu.id));
	APPEL_GX(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	APPEL_GX(glDrawBuffer(GL_FRONT_AND_BACK));
	APPEL_GX(glReadBuffer(GL_COLOR_ATTACHMENT0));

	APPEL_GX(glBlitFramebuffer(0, 0, dimensionBackbuffer.x, dimensionBackbuffer.y, 0, 0, dimensionBackbuffer.x, dimensionBackbuffer.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST));

	Framebuffer::delier();
}