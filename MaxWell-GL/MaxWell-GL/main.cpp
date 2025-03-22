#include "Application/Application.h"
#include "Application/Utilitaire.h"

#include "Moteur Graphique/MoteurGx/ListeIndexUnique.h"

/*#define GLEW_STATIC
#include "Moteur Graphique/Graphique.h"
#include "Lib/GLM/glm/gtc/matrix_transform.hpp"

#include "Lib/GLFW/include/GLFW/glfw3.h"

#include "Moteur Graphique/Shader/Shader.h"
#include "Moteur Graphique/Model/DecoderFichier.h"
#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"
#include "Moteur Graphique/Vertexarray/Vertexarray.h"
#include "Moteur Graphique/Framebuffer/Framebuffer.h"
#include "Moteur Graphique/Texture/Texture.h"
#include "Moteur Graphique/Pipeline/Pipeline.h"

#include "Application/Application.h"

#include <string>
#include <conio.h>
#include <chrono>

glm::vec3 orientation(float yaw, float pitch) {
	float yawR = glm::radians(yaw);
	float pitchR = glm::radians(pitch);

	return glm::vec3(sin(yawR) * cos(pitchR), sin(pitchR), cos(yawR) * cos(pitchR));
}*/

/*int main() {
	printf("allo le monde!");

	uint32_t largeur = 800;
	uint32_t hauteur = 600;

	// Initialisation de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Création de la fenêtre avec GLFW

	GLFWwindow* fenetre = glfwCreateWindow(largeur, hauteur, "OPENGL 3D", NULL, NULL);
	if (fenetre == NULL) {
		printf("ERREUR | Erreur dans la création de la fenetre GLFW\n");

		glfwTerminate();

		_getch();
		return -1;
	}

	printf("LOG | Creation de la fenetre avec GLFW\n");

	glfwMakeContextCurrent(fenetre);

	glfwSwapInterval(1);

	// Initialisation de GLEW

	if (glewInit() != GLEW_OK) {
		printf("ERREUR | Erreur dans l'initialisation de GLEW\n");

		_getch();
		return -1;
	}

	printf("LOG | Initialisation de GLEW\n");

	printf("LOG | OPENGL vendor : %s\n", glGetString(GL_VENDOR));
	printf("LOG | OPENGL renderer : %s\n", glGetString(GL_RENDERER));
	printf("LOG | OPENGL version : %s\n", glGetString(GL_VERSION));
	printf("LOG | OPENGL Supported glsl version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	Shader shader;
	Shader::generer(&shader);
	{
		std::string shaderData;
		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Tesselation surface\\Surface vertex.glsl", &shaderData);
		Shader::loadSubShader(shader, shaderData, SHADER_VERTEX);

		shaderData.clear();

		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Tesselation surface\\Surface tessControle.glsl", &shaderData);
		Shader::loadSubShader(shader, shaderData, SHADER_CONTROLE_TESSELATION);

		shaderData.clear();

		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Tesselation surface\\Surface tessEval.glsl", &shaderData);
		Shader::loadSubShader(shader, shaderData, SHADER_EVAL_TESSELATION);

		shaderData.clear();

		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Tesselation surface\\Surface geometrie.glsl", &shaderData);
		Shader::loadSubShader(shader, shaderData, SHADER_GEOMETRIE);

		shaderData.clear();

		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Tesselation surface\\Surface fragment.glsl", &shaderData);
		Shader::loadSubShader(shader, shaderData, SHADER_FRAGMENT);
	}
	Shader::assembler(shader);

	Shader shaderCalculOnde;
	Shader::generer(&shaderCalculOnde);
	{
		std::string shaderData;
		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Calcul onde\\Calcul onde.glsl", &shaderData);

		Shader::loadSubShader(shaderCalculOnde, shaderData, SHADER_CALCUL);
	}
	Shader::assembler(shaderCalculOnde);
	
	Shader shaderCalculGradientOnde;
	Shader::generer(&shaderCalculGradientOnde);
	{
		std::string shaderData;
		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Calcul onde\\Calcul gradient onde.glsl", &shaderData);

		Shader::loadSubShader(shaderCalculGradientOnde, shaderData, SHADER_CALCUL);
	}
	Shader::assembler(shaderCalculGradientOnde);

	Shader shaderCalculGradient2Onde;
	Shader::generer(&shaderCalculGradient2Onde);
	{
		std::string shaderData;
		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Calcul onde\\Calcul gradient2 onde.glsl", &shaderData);

		Shader::loadSubShader(shaderCalculGradient2Onde, shaderData, SHADER_CALCUL);
	}
	Shader::assembler(shaderCalculGradient2Onde);

	Model carre;
	carre.nbTriangle = 2;
	carre.triangles = new Triangle[carre.nbTriangle];
	carre.triangles[0].verts[0].pos = glm::vec3(-1, -1, 0);
	carre.triangles[0].verts[1].pos = glm::vec3(-1,  1, 0);
	carre.triangles[0].verts[2].pos = glm::vec3( 1,  1, 0);
	carre.triangles[1].verts[0].pos = glm::vec3( 1,  1, 0);
	carre.triangles[1].verts[1].pos = glm::vec3( 1, -1, 0);
	carre.triangles[1].verts[2].pos = glm::vec3(-1, -1, 0);

	Vertexbuffer vbo;
	Vertexbuffer::generer(&vbo, 20);
	Vertexbuffer::transfererDonnees(vbo, 0, sizeof(carre), carre.triangles);

	Vertexarray vao;
	Vertexarray::generer(&vao);
	Vertexbuffer::lier(vbo);
	Vertexarray::ajouterAttribut(vao, 0, 3, TYPE_VIRGULE, TYPE_FAUX, sizeof(Vertex), offsetof(Vertex, Vertex::pos));

	const glm::uvec2 tailleCarte = glm::uvec2(100, 200);

	glm::vec2* pointOriginaux = new glm::vec2[tailleCarte.x * tailleCarte.y];

	for (int i = 0; i < tailleCarte.x * tailleCarte.y; i++)
	{
		pointOriginaux[i] = glm::vec2(0.0f);
	}

	pointOriginaux[tailleCarte.x * tailleCarte.y / 2 + tailleCarte.x / 2] = glm::vec2(0.0f, 20.0f);

	Texture carteHauteurCorde;
	Texture::generer(&carteHauteurCorde, tailleCarte.x, tailleCarte.y, pointOriginaux, GL_RG16F, TEX_FORMAT_RV, TYPE_VIRGULE);
	Texture::specifierCouleurBordure(carteHauteurCorde, glm::vec4(0));
	Texture::specifierFiltre(carteHauteurCorde, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);
	Texture::specifierEtirement(carteHauteurCorde, TEX_ENVELOPPER_LIMITER_BORD, TEX_ENVELOPPER_LIMITER_BORD);

	Texture carteGradientCorde;
	Texture::generer(&carteGradientCorde, tailleCarte.x, tailleCarte.y, nullptr, GL_R16F, TEX_FORMAT_ROUGE, TYPE_VIRGULE);
	Texture::specifierCouleurBordure(carteGradientCorde, glm::vec4(0));
	Texture::specifierFiltre(carteGradientCorde, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);
	Texture::specifierEtirement(carteGradientCorde, TEX_ENVELOPPER_LIMITER_BORD, TEX_ENVELOPPER_LIMITER_BORD);

	Texture carteGradient2Corde;
	Texture::generer(&carteGradient2Corde, tailleCarte.x, tailleCarte.y, nullptr, GL_R16F, TEX_FORMAT_ROUGE, TYPE_VIRGULE);
	Texture::specifierCouleurBordure(carteGradient2Corde, glm::vec4(0));
	Texture::specifierFiltre(carteGradient2Corde, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);
	Texture::specifierEtirement(carteGradient2Corde, TEX_ENVELOPPER_LIMITER_BORD, TEX_ENVELOPPER_LIMITER_BORD);

	Framebuffer fbo;
	Framebuffer::generer(&fbo);
	Framebuffer::addAttachment(&fbo, largeur, hauteur, TEX_INFORMAT_RVBA, TEX_FORMAT_RVBA, TYPE_VIRGULE, TEX_FILTRE_PROCHE, TEX_FILTRE_PROCHE);

	Pipeline renduSurface(&fbo, &shader);
	renduSurface.equationMelange = GL_FUNC_ADD;
	renduSurface.modeMelangeSRC = GL_SRC_ALPHA;
	renduSurface.modeMelangeDST = GL_ONE_MINUS_SRC_ALPHA;
	renduSurface.testProfondeur = GL_LESS;
	renduSurface.modeEliminationFace = GL_BACK;
	renduSurface.tailleFenetre = glm::uvec2(largeur, hauteur);
	renduSurface.stencilFunc = FUNC_TOUJOURS;
	renduSurface.stencilMasque = 0xFF;
	renduSurface.stencilRef = 0xFF;
	renduSurface.stencilEchec = STENCIL_FUNC_GARDER;
	renduSurface.profondeurEchec = STENCIL_FUNC_GARDER;
	renduSurface.stencilProfondeurReussite = STENCIL_FUNC_GARDER;
	
	auto tAvant = std::chrono::high_resolution_clock::now();

	const float aspectRatio = (float)largeur / hauteur;

	float yaw = 0, pitch = 0;

	while (!glfwWindowShouldClose(fenetre))
	{
		auto tMaintenant = std::chrono::high_resolution_clock::now();
		float dt = (tMaintenant - tAvant).count() / 1000000000.0f;
		tAvant = tMaintenant;

		yaw += (glfwGetKey(fenetre, GLFW_KEY_RIGHT) - glfwGetKey(fenetre, GLFW_KEY_LEFT)) * dt * 50;
		pitch += (glfwGetKey(fenetre, GLFW_KEY_UP) - glfwGetKey(fenetre, GLFW_KEY_DOWN)) * dt * 50;

		Framebuffer::lier(fbo);

		APPEL_GX(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		APPEL_GX(glClear(GL_COLOR_BUFFER_BIT));

		APPEL_GX(glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE));

		Shader::lier(shaderCalculGradientOnde);
		APPEL_GX(glBindImageTexture(0, carteGradientCorde.id, 0, TYPE_FAUX, 0, GL_WRITE_ONLY, GL_R16F));
		APPEL_GX(glBindImageTexture(1, carteHauteurCorde.id, 0, TYPE_FAUX, 0, GL_READ_ONLY, GL_RG16F));

		Shader::pousserConstanteVec2(shaderCalculGradientOnde, "u_invDs", glm::vec2(tailleCarte));

		APPEL_GX(glDispatchCompute(tailleCarte.x, tailleCarte.y, 1));
		APPEL_GX(glMemoryBarrier(GL_ALL_BARRIER_BITS));

		Shader::lier(shaderCalculGradient2Onde);
		APPEL_GX(glBindImageTexture(0, carteGradient2Corde.id, 0, TYPE_FAUX, 0, GL_WRITE_ONLY, GL_R16F));
		APPEL_GX(glBindImageTexture(1, carteGradientCorde.id, 0, TYPE_FAUX, 0, GL_READ_ONLY, GL_R16F));

		APPEL_GX(glDispatchCompute(tailleCarte.x, tailleCarte.y, 1));
		APPEL_GX(glMemoryBarrier(GL_ALL_BARRIER_BITS));

		////////////////////////////////////////////////////////////////////

		Shader::lier(shaderCalculOnde);
		Shader::pousserConstanteVirgule(shaderCalculOnde, "u_dt", dt);
		APPEL_GX(glBindImageTexture(0, carteHauteurCorde.id, 0, TYPE_FAUX, 0, GL_READ_WRITE, GL_RG16F));
		APPEL_GX(glBindImageTexture(1, carteGradient2Corde.id, 0, TYPE_FAUX, 0, GL_READ_ONLY, GL_R16F));

		APPEL_GX(glDispatchCompute(tailleCarte.x, tailleCarte.y, 1));
		APPEL_GX(glMemoryBarrier(GL_ALL_BARRIER_BITS));

		////////////////////////////////////////////////////////////////////
	
		Pipeline::init(renduSurface);

		glm::vec3 posCam = orientation(yaw, pitch) * 10.0f;

		glm::mat4 vueCam = glm::perspective(70.0f, aspectRatio, 1.0f, -1.0f) * glm::lookAt(posCam, glm::vec3(0.0f), glm::vec3(0, 1, 0));

		Shader::pousserConstanteMat4(shader, "u_cam", vueCam);

		glActiveTexture(GL_TEXTURE0);
		//Texture::lier(carteGradientCorde);
		Texture::lier(carteHauteurCorde);
		Shader::pousserTexture(shader, "u_cartePoint", 0);
		glActiveTexture(GL_TEXTURE1);
		Texture::lier(carteGradientCorde);
		Shader::pousserTexture(shader, "u_carteDiv", 1);
		Shader::pousserConstanteUVec2(shader, "u_tailleCarte", tailleCarte);
		Shader::pousserConstanteVec3(shader, "u_posCam", posCam);

		Pipeline::dessiner(renduSurface, vao, carre);

		////////////////////////////////////////////////////////////////////

		APPEL_GX(glClampColor(GL_CLAMP_READ_COLOR, GL_TRUE));

		APPEL_GX(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo.id));
		APPEL_GX(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		APPEL_GX(glBlitFramebuffer(0, 0, largeur, hauteur, 0, 0, largeur, hauteur, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST));

		glfwSwapBuffers(fenetre);
		glfwPollEvents();
	}

	glfwTerminate();

	delete[] carre.triangles;

	_getch();

	return -1;
}*/

int main() {
	Application application;

	Application::initialiser(&application, glm::uvec2(800, 600));
	
	Application::executer(application);
	
	Application::fermer(&application);

	return 0;
}