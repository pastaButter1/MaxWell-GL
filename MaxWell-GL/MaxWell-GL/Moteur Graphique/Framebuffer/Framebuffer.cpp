#include "Framebuffer.h"

void Framebuffer::generer(Framebuffer* const fbo)
{
	APPEL_GX(glGenFramebuffers(1, (GLuint*)&(fbo->id)));
}

void Framebuffer::detruire(Framebuffer* const fbo)
{
	APPEL_GX(glDeleteFramebuffers(1, (GLuint*)&(fbo->id)));
}

void Framebuffer::addAttachment(Framebuffer* const fbo, uint32_t largeur, uint32_t hauteur, uint32_t internalFormat, uint32_t format, uint32_t dataType, uint32_t minFilter, uint32_t magFilter)
{
	uint32_t nbAttachments = fbo->infoAttachments & nbAttachmentMask;
	bool depthStencil = fbo->infoAttachments & masqueProfondeurStencil >> _tzcnt_u32(masqueProfondeurStencil);
	uint32_t colorAttachments = (fbo->infoAttachments & masqueCouleur) >> _tzcnt_u32(masqueCouleur);

	int texIndex = 0;
	uint32_t attachmentType = FBO_ATTACHEMENT_COULEUR0;
	switch (format) {
	case TEX_COMPOSANT_PROFONDEUR:
		attachmentType = FBO_ATTACHEMENT_PROFONDEUR;
		break;
	case TEX_INDEX_STENCIL:
		attachmentType = FBO_ATTACHEMENT_STENCIL;
		break;
	case TEX_PROFONDEUR_STENCIL:
		attachmentType = FBO_ATTACHEMENT_PROFONDEUR_STENCIL;
		break;
	default:
		for (uint32_t mask = 1 << (__popcnt(masqueCouleur) - 1); texIndex <= __popcnt(masqueCouleur) + 1; texIndex++, mask >>= 1) {
			bool bit = colorAttachments & mask;

			if (!bit) {
				break;
			}
		}

		attachmentType += texIndex;
		texIndex += 1;
		break;
	}

	Texture& tex = fbo->textureAttachments[texIndex];

	Texture::generer(&tex, GL_TEXTURE_2D, internalFormat); /* TODO À changer */
	Texture::allouer2D(tex, 0, glm::ivec2(largeur, hauteur), format, dataType, nullptr);

	Texture::specifierEtirement(tex, TEX_ENVELOPPER_LIMITER_BORDURE, TEX_ENVELOPPER_LIMITER_BORDURE, TEX_ENVELOPPER_LIMITER_BORDURE);

	uint32_t zeros[] = { 0, 0, 0, 0 };

	Texture::specifierCouleurBordure(tex, *(glm::vec4*)zeros);

	Texture::specifierFiltre(tex, GL_NEAREST, GL_NEAREST);

	lier(*fbo);
	APPEL_GX(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TEX_2D, tex.id, 0));

	uint32_t fboStatus = APPEL_GX(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (fboStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		if (attachmentType == FBO_ATTACHEMENT_PROFONDEUR || attachmentType == FBO_ATTACHEMENT_STENCIL || attachmentType == FBO_ATTACHEMENT_PROFONDEUR_STENCIL)
		{
			fbo->infoAttachments |= masqueProfondeurStencil;
		}
		else 
		{
			fbo->infoAttachments |= 1 << (__popcnt(masqueCouleur) - texIndex + _tzcnt_u32(masqueCouleur));
		}

		fbo->infoAttachments++;

		return;
	}

	printf("ERROR | Erreur durant l'ajout d'un attachement au framebuffer : %i\n", fboStatus);
}

void Framebuffer::lier(const Framebuffer& fbo)
{
	APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, fbo.id));
}

void Framebuffer::delier()
{
	APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
