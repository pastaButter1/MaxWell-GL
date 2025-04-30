#include "MoteurGX.h"

#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"

using namespace mgx;

void mgx::Pipeline::init(const Pipeline& pipeline, const Framebuffer& fbo, const Shader& shader)
{
	Framebuffer::lier(fbo);
	Shader::lier(shader);

	APPEL_GX(glViewport(0, 0, (EnumGX)pipeline.tailleFenetre.x, (EnumGX)pipeline.tailleFenetre.y));
	APPEL_GX(glScissor(0, 0, (EnumGX)pipeline.tailleFenetre.x, (EnumGX)pipeline.tailleFenetre.y));

	switch (pipeline.testProfondeur)
	{
	case Operation::Profondeur::DESACTIVER:
		APPEL_GX(glDisable((EnumGX)Operation::Operation::PROFONDEUR));
		break;
	default:
		APPEL_GX(glEnable((EnumGX)Operation::Operation::PROFONDEUR));
		APPEL_GX(glDepthFunc((EnumGX)pipeline.testProfondeur));
		if (pipeline.testProfondeur == Operation::Profondeur::LECTURE)
		{
			APPEL_GX(glDepthMask((EnumGX)Donnee::Binaire::FAUX));
		}
		else
		{
			APPEL_GX(glDepthMask((EnumGX)Donnee::Binaire::VRAI));
		}
		break;
	}

	// TODO À finir pour stencil

	switch (pipeline.stencilFunc)
	{
	case Operation::Stencil::DESACTIVER:
		APPEL_GX(glDisable((EnumGX)Operation::Operation::STENCIL));
		break;
	case Operation::Stencil::LECTURE:
		APPEL_GX(glEnable((EnumGX)Operation::Operation::STENCIL));
		APPEL_GX(glStencilMask((EnumGX)Donnee::Binaire::FAUX));
		break;
	default:
		APPEL_GX(glEnable((EnumGX)Operation::Operation::STENCIL));
		APPEL_GX(glStencilMask((EnumGX)Donnee::Binaire::VRAI));
		APPEL_GX(glStencilFunc((EnumGX)pipeline.stencilFunc, (EnumGX)pipeline.stencilRef, (EnumGX)pipeline.stencilMasque));
		APPEL_GX(glStencilOp((EnumGX)pipeline.stencilEchec, (EnumGX)pipeline.profondeurEchec, (EnumGX)pipeline.stencilProfondeurReussit));
	}

	if (pipeline.nettoyerCible)
	{
		APPEL_GX(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		APPEL_GX(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	
	if (pipeline.modeCulling == Operation::Culling::DESACTIVER)
	{
		APPEL_GX(glDisable((EnumGX)Operation::Operation::CULLING));
	}
	else
	{
		APPEL_GX(glEnable((EnumGX)Operation::Operation::CULLING));
		APPEL_GX(glCullFace((EnumGX)pipeline.modeCulling));
	}

	// Definir les cibles du dessinage

	const uint32_t masqueAttachements = Framebuffer::masqueCouleur;
	const uint32_t listeAttachement = fbo.infoAttachments & (masqueAttachements);

	Fbo::Attachement destinations[Framebuffer::nbAttachmentMask];
	uint32_t nbAttachements = 0;
	for (uint32_t masque = 0x80000000, i = 0; i < __popcnt(masqueAttachements); masque >>= 1, i++)
	{
		uint32_t a = listeAttachement & masque;
		if ((listeAttachement & masque) != 0)
		{
			destinations[nbAttachements] = (Fbo::Attachement)((EnumGX)Fbo::Attachement::COULEUR0 + nbAttachements);
			nbAttachements++;
		}
	}

	if (pipeline.equationMelange == Operation::Melange::DESACTIVER)
	{
		APPEL_GX(glDrawBuffers(0, (uint32_t*)destinations));
	}

	else
	{
		APPEL_GX(glBlendFunc((EnumGX)pipeline.modeMelangeSRC, (EnumGX)pipeline.modeMelangeDST));
		APPEL_GX(glBlendEquation((EnumGX)pipeline.equationMelange));
		APPEL_GX(glDrawBuffers(nbAttachements, (uint32_t*)destinations));
	}
}

void mgx::Pipeline::dessiner(const Pipeline& pipeline, const Vertexarray vao)
{
	Vertexarray::lier(vao);
	APPEL_GX(glDrawArrays((EnumGX)pipeline.modeDessin, 0, vao.nbTriangles * 3));
}

void MoteurGX::init(MoteurGX* const mGX)
{
	APPEL_GX(glEnable((EnumGX)Operation::Operation::MELANGE));
	APPEL_GX(glEnable((EnumGX)Operation::Operation::PROFONDEUR));
	APPEL_GX(glEnable((EnumGX)Operation::Operation::STENCIL));
	APPEL_GX(glEnable((EnumGX)Operation::Operation::CULLING));
	APPEL_GX(glEnable((EnumGX)Operation::Operation::CISEAU));

	Ressource fboIU, texCoulIU, texProfIU;
	Framebuffer& fbo = MoteurGX::creerFramebuffer(mGX, &fboIU);
	MoteurGX::creerTexture(mGX, &texCoulIU);
	Texture& texProfondeur = MoteurGX::creerTexture(mGX, &texProfIU);
	Texture& texCouleur = MoteurGX::retTexture(*mGX, texCoulIU);
	Framebuffer::addAttachment(&fbo, &texCouleur, 800, 600, Tex::FormatInterne::RVBA, Tex::Format::RVBA, Donnee::Type::U32, Tex::Filtre::PROCHE, Tex::Filtre::PROCHE);
	Framebuffer::addAttachment(&fbo, &texProfondeur, 800, 600, Tex::FormatInterne::COMPOSANT_PROFONDEUR, Tex::Format::COMPOSANT_PROFONDEUR, Donnee::Type::U32, Tex::Filtre::PROCHE, Tex::Filtre::PROCHE);
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

const Shader& MoteurGX::demarerProgramme(const MoteurGX& mGX, const Ressource IndexPipeline)
{
	const Pipeline& pipeline = mGX.listePipelines.rechercherIndexUnique(IndexPipeline);
	const Framebuffer& fbo = MoteurGX::retFBO(mGX, pipeline.fbo);
	const Shader& shader = MoteurGX::retShader(mGX, pipeline.shader);

	Pipeline::init(pipeline, fbo, shader);

	return shader;

}

void MoteurGX::executerProgramme(const MoteurGX& mGX, const Ressource pipelineIU, const Ressource vaoIU)
{
	const Couche& couche = mGX.coucheActive;

	const Pipeline& pipeline = MoteurGX::retPipeline(mGX, pipelineIU);

	//APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	const Vertexarray& vao = retVertexarray(mGX, vaoIU);

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