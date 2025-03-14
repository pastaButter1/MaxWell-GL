#include "Pipeline.h"

void Pipeline::init(const Pipeline& pipeline)
{
	Framebuffer::lier(*pipeline.fbo);
	Shader::lier(*pipeline.shader);

	APPEL_GX(glViewport(0, 0, pipeline.tailleFenetre.x, pipeline.tailleFenetre.y));
	APPEL_GX(glScissor(0, 0, pipeline.tailleFenetre.x, pipeline.tailleFenetre.y));
	APPEL_GX(glCullFace(pipeline.modeEliminationFace));
	APPEL_GX(glDepthFunc(pipeline.testProfondeur));
	APPEL_GX(glBlendFunc(pipeline.modeMelangeSRC, pipeline.modeMelangeDST));
	APPEL_GX(glBlendEquation(pipeline.equationMelange));

	APPEL_GX(glStencilFunc(pipeline.stencilFunc, pipeline.stencilRef, pipeline.stencilMasque));
	APPEL_GX(glStencilOp(pipeline.stencilEchec, pipeline.profondeurEchec, pipeline.stencilProfondeurReussite));
}

void Pipeline::dessiner(const Pipeline& pipeline, const Vertexarray vao)
{ 
	Vertexarray::lier(vao);
	APPEL_GX(glDrawArrays(GL_PATCHES, 0, vao.nbTriangles * 3));
}
