#include "Framebuffer.h"

void Framebuffer::generer(Framebuffer* const fbo)
{
	APPEL_GX(glGenFramebuffers(1, (GLuint*)&(fbo->id)));
}

void Framebuffer::detruire(Framebuffer* const fbo)
{
	APPEL_GX(glDeleteFramebuffers(1, (GLuint*)&(fbo->id)));
}

void Framebuffer::addAttachment(Framebuffer* const fbo, Texture* const tex, const uint32_t largeur, const uint32_t hauteur, const Tex::FormatInterne formatInterne, const Tex::Format format, const Donnee::Type typeDonnees, const Tex::Filtre filtreMin, const Tex::Filtre filtreMag)
{
	uint32_t nbAttachments = fbo->infoAttachments & nbAttachmentMask;
	bool depthStencil = fbo->infoAttachments & masqueProfondeurStencil >> _tzcnt_u32(masqueProfondeurStencil);
	uint32_t colorAttachments = (fbo->infoAttachments & masqueCouleur) >> _tzcnt_u32(masqueCouleur);

	int texIndex = 0;
	Fbo::Attachement typeAttachement = Fbo::Attachement::COULEUR0;
	switch (format) {
	case Tex::Format::COMPOSANT_PROFONDEUR:
		typeAttachement = Fbo::Attachement::PROFONDEUR;
		break;
	case Tex::Format::INDEX_STENCIL:
		typeAttachement = Fbo::Attachement::STENCIL;
		break;
	case Tex::Format::PROFONDEUR_STENCIL:
		typeAttachement = Fbo::Attachement::PROFONDEUR_STENCIL;
		break;
	default:
		for (uint32_t mask = 1 << (__popcnt(masqueCouleur) - 1); texIndex <= __popcnt(masqueCouleur) + 1; texIndex++, mask >>= 1) {
			bool bit = colorAttachments & mask;

			if (!bit) {
				break;
			}
		}

		*(uint32_t*)&typeAttachement += texIndex;
		texIndex += 1;
		break;
	}

	//Texture& tex = fbo->textureAttachments[texIndex];

	Texture::allouer2D(tex, 0, glm::ivec2(largeur, hauteur), formatInterne, format, typeDonnees, nullptr);

	Texture::specifierEtirement(*tex, Tex::Emballage::LIMITER_BORDURE, Tex::Emballage::LIMITER_BORDURE, Tex::Emballage::LIMITER_BORDURE);

	uint32_t zeros[] = { 0, 0, 0, 0 };

	Texture::specifierCouleurBordure(*tex, *(glm::vec4*)zeros);

	Texture::specifierFiltre(*tex, Tex::Filtre::PROCHE, Tex::Filtre::PROCHE);

	lier(*fbo);
	APPEL_GX(glFramebufferTexture2D(GL_FRAMEBUFFER, (EnumGX)typeAttachement, GL_TEXTURE_2D, tex->id, 0));

	uint32_t fboStatus = APPEL_GX(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (fboStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		if (typeAttachement == Fbo::Attachement::PROFONDEUR || typeAttachement == Fbo::Attachement::STENCIL || typeAttachement == Fbo::Attachement::PROFONDEUR_STENCIL)
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
