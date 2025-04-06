#include "MoteurPhysique.h"
#include "Moteur Graphique/Model/DecoderFichier.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"

#include "MoteurPhysique.h"


void MoteurPhysique::ChargerShaders(MoteurPhysique* const moteurPhysique, const std::string& cheminDossier)
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

void MoteurPhysique::InitialiserFils(Info* const info)
{
	APPEL_GX(glCreateBuffers(1, &info->buffer));
}

void MoteurPhysique::SoumettreFils(const Info& info)
{
	APPEL_GX(glBindBuffer(GL_SHADER_STORAGE_BUFFER, info.buffer));
	APPEL_GX(glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Fil) * info.fils.size(), info.fils.data(), GL_STATIC_READ));
}

void MoteurPhysique::CalculerGPU(const Shader& shader, const Espace & coordonnees, const Espace& donnees, const Info& info)
{
	APPEL_GX(glUseProgram(shader.id));

	APPEL_GX(glUniform1i(glGetUniformLocation(shader.id, "nombreFils"), info.fils.size()));
	Texture::attacherImage(coordonnees.tex, 0, GL_READ_ONLY);
	Texture::attacherImage(donnees.tex, 1, GL_WRITE_ONLY);
	APPEL_GX(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, info.buffer));

	//Local (10,10,1)
	APPEL_GX(glDispatchCompute(coordonnees.dimensions.x / 10, coordonnees.dimensions.y / 10, coordonnees.dimensions.z));
}

void MoteurPhysique::AssignerCoordonnees(const MoteurPhysique& moteurPhysique, glm::vec3 min, glm::vec3 max)
{
	glUseProgram(moteurPhysique.shaderCoordonnees.id);

	glUniform3iv(glGetUniformLocation(moteurPhysique.shaderCoordonnees.id, "dimension"), 1, &moteurPhysique.coordonnees.dimensions.x);
	glUniform3fv(glGetUniformLocation(moteurPhysique.shaderCoordonnees.id, "min"), 1, &min.x);
	glUniform3fv(glGetUniformLocation(moteurPhysique.shaderCoordonnees.id, "max"), 1, &max.x);
	Texture::attacherImage(moteurPhysique.coordonnees.tex, 0, GL_WRITE_ONLY);

	//Local (10,10,1)
	glDispatchCompute(moteurPhysique.coordonnees.dimensions.x / 10, moteurPhysique.coordonnees.dimensions.y / 10, moteurPhysique.coordonnees.dimensions.z);
}
