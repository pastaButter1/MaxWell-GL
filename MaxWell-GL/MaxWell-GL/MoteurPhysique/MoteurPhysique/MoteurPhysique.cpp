#include "MoteurPhysique.h"
#include "Moteur Graphique/Model/DecoderFichier.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"

#include "MoteurPhysique.h"


void MoteurPhysique::GPU::chargerShaders(MoteurPhysique* const moteurPhysique, const std::string& cheminDossier)
{
	std::string contenu;
	chargerFichier(cheminDossier + "Coordonnees.glsl", &contenu);

	Shader::generer(&moteurPhysique->shaderCoordonnees);
	Shader::loadSubShader(moteurPhysique->shaderCoordonnees, contenu, GL_COMPUTE_SHADER);
	Shader::assembler(moteurPhysique->shaderCoordonnees);
	
	chargerFichier(cheminDossier + "ChampMagnetique.glsl", &contenu);

	Shader::generer(&moteurPhysique->shaderChampMagnetique);
	Shader::loadSubShader(moteurPhysique->shaderChampMagnetique, contenu, GL_COMPUTE_SHADER);
	Shader::assembler(moteurPhysique->shaderChampMagnetique);
}

void MoteurPhysique::GPU::genererBufferInfo(Info* const info)
{
	APPEL_GX(glCreateBuffers(2, info->buffers));
}

void MoteurPhysique::GPU::soumettreBufferInfo(const Info& info)
{
	uint32_t nb = info.fils.size();
	APPEL_GX(glBindBuffer(GL_SHADER_STORAGE_BUFFER, info.buffers[0]));
	APPEL_GX(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Fil) * nb + 16, nullptr, GL_STATIC_READ));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(nb), &nb);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(Fil) * nb, info.fils.data());

	nb = info.solenoides.size();
	APPEL_GX(glBindBuffer(GL_SHADER_STORAGE_BUFFER, info.buffers[1]));
	APPEL_GX(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Solenoide) * nb + 16, nullptr, GL_STATIC_READ));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(nb), &nb);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(Solenoide) * nb, info.solenoides.data());
}

void MoteurPhysique::GPU::executerCalcul(const Shader& shader, const Espace & coordonnees, const Espace& donnees, const Info& info)
{
	APPEL_GX(glUseProgram(shader.id));

	Texture::attacherImage(coordonnees.tex, 0, GL_READ_ONLY);
	Texture::attacherImage(donnees.tex, 1, GL_WRITE_ONLY);
	APPEL_GX(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, info.buffers[0]));
	APPEL_GX(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, info.buffers[1]));

	//Local (10,10,1)
	APPEL_GX(glDispatchCompute(coordonnees.dimensions.x / 10, coordonnees.dimensions.y / 10, coordonnees.dimensions.z));

	APPEL_GX(glMemoryBarrier(GL_ALL_BARRIER_BITS));
}

void MoteurPhysique::GPU::assignerCoordonnees(const MoteurPhysique& moteurPhysique, glm::vec3 min, glm::vec3 max)
{
	glUseProgram(moteurPhysique.shaderCoordonnees.id);

	glUniform3iv(glGetUniformLocation(moteurPhysique.shaderCoordonnees.id, "dimension"), 1, &moteurPhysique.coordonnees.dimensions.x);
	glUniform3fv(glGetUniformLocation(moteurPhysique.shaderCoordonnees.id, "min"), 1, &min.x);
	glUniform3fv(glGetUniformLocation(moteurPhysique.shaderCoordonnees.id, "max"), 1, &max.x);
	Texture::attacherImage(moteurPhysique.coordonnees.tex, 0, GL_WRITE_ONLY);

	//Local (10,10,1)
	glDispatchCompute(moteurPhysique.coordonnees.dimensions.x / 10, moteurPhysique.coordonnees.dimensions.y / 10, moteurPhysique.coordonnees.dimensions.z);
	APPEL_GX(glMemoryBarrier(GL_ALL_BARRIER_BITS));
}

MoteurPhysique::Fil MoteurPhysique::Fil::creer(glm::vec3 direction, glm::vec3 origine, float courant)
{
	return { direction, courant, origine };
}

MoteurPhysique::Solenoide MoteurPhysique::Solenoide::creer(glm::vec3 direction, glm::vec3 origine, float courant, float nombreTours, float longeur)
{
	return { direction, courant, origine, nombreTours, longeur};
}
