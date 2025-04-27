#include "Application.h"
#include "Moteur Graphique/Graphique.h"
#include "Lib/GLFW/include/GLFW/glfw3.h"

#include "Lib/IMGUI/IMGUI/imgui.h"
#include "Lib/IMGUI/IMGUI/imgui_impl_glfw.h"
#include "Lib/IMGUI/IMGUI/imgui_impl_opengl3.h"
#include "Lib/STBImage/stb_image.h"

#include "Utilitaire.h"

#include <conio.h>
#include <chrono>

#include "Moteur Graphique/Model/DecoderFichier.h"
#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"
#include "Moteur Graphique/MoteurGx/Mesh.h"
#include "Moteur Graphique/MoteurGx/MoteurGX.h"
#include "Moteur Graphique/Camera/Camera.h"

#include "Lib/GLM/glm/matrix.hpp"
#include "Lib/GLM/glm/gtc/matrix_transform.hpp"

#include "MoteurPhysique/MoteurPhysique/MoteurPhysique.h"

using Ressource = MoteurGX::Ressource;

void genererSolenoide(Model* const model, const uint32_t nbTriangles, const float R, const float r, const float l, const float rot);

void Application::initialiser(Application* const app, glm::uvec2 tailleFenetre)
{
	Fenetre::init(&app->fenetre, tailleFenetre);

	initialiserInterfaceUtilisateur(app);

	initialiserMoteurGraphique(app);
}

void Application::executer(Application* const app)
{
	auto tAvant = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(app->fenetre.window))
	{
		auto tMaintenant = std::chrono::high_resolution_clock::now();
		const float dt = (tMaintenant - tAvant).count() / 1000000000.0f;
		tAvant = tMaintenant;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_None;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::Begin("Dockspace window", 0, window_flags);

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace, ImVec2(0, 0), dockspace_flags);
		}

		ImGui::End();

		ImGui::Begin("Edition");

		ImGui::SliderFloat("Dis Z", &app->donnesOperation.disZ, -10.0f, 10.0f);
		ImGui::SliderFloat("Lumiere Y", &app->donnesOperation.angleLumiere.x, 0.0f, 360.0f);
		ImGui::SliderFloat("Lumiere Z", &app->donnesOperation.angleLumiere.y, -90.0f, 90.0f);

		auto& solFin = app->donnesOperation.solenoide;
		auto solInit = solFin;

		ImGui::SliderFloat("Solenoide L", &solFin.longueur, 0.0f, 30.0f);
		ImGui::SliderFloat("Solenoide N", &solFin.nbRot, 0.1f, 10.0f);
		ImGui::SliderFloat("Solenoide R", &solFin.R, 0.0f, 15.0f);
		ImGui::SliderFloat("Solenoide r", &solFin.r, 0.0f, 1.0f);

		if (memcmp(&solInit, &solFin, sizeof(solFin)) != 0)
		{
			const float L = app->donnesOperation.solenoide.longueur;
			const float R = app->donnesOperation.solenoide.R;
			const float r = app->donnesOperation.solenoide.r;
			const float rot = app->donnesOperation.solenoide.nbRot;

			Model model;
			genererSolenoide(&model, 30000, R, r, L, rot);
			mgx::Mesh::chargerModel<Vertex>(&app->donnesOperation.meshSolenoide, &app->moteurGX, model.nbTriangle * 3, model.triangles);
			delete[] model.triangles;
		}

		ImGui::SliderFloat("Ambient", &app->donnesOperation.kAmbient, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse", &app->donnesOperation.kDiffuse, 0.0f, 1.0f);
		ImGui::SliderFloat("Speculaire", &app->donnesOperation.kSpeculaire, 0.0f, 1.0f);
		ImGui::SliderFloat("Exposant", &app->donnesOperation.esposantSpec, 0.0f, 1.0f);
		ImGui::ColorEdit3("Couleur", (float*)&app->donnesOperation.couleur);

		ImGui::End();

		ImGui::Begin("fenetre");

		static glm::vec4 couleur;

		//ImGui::ColorEdit4("Couleur", (float*)&couleur);
		ImGui::Image((void*)MoteurGX::retTexture(app->moteurGX, 0).id, ImVec2(800, 600), ImVec2(1, 1), ImVec2(0, 0));

		ImGui::End();

		executerEntrees(app, dt);
		executerRendu(app);
		//MoteurGX::copierRenduBackbuffer(app->moteurGX, glm::uvec2(800, 600));

		Framebuffer::delier();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(app->fenetre.window);
		glfwPollEvents();
	}
}

void Application::fermer(Application* const app)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(app->fenetre.window);
	glfwTerminate();

	app->fenetre.window = nullptr;
}

void genererSolenoide(Model* const model, const uint32_t nbTriangles, const float R, const float r, const float L, const float rot)
{
	Model& m = *model;

	const uint32_t nbTranches = rot * sqrt(nbTriangles);
	const uint32_t nbTriSurface = (nbTriangles / nbTranches) & (~1);
	const uint32_t nbPointCirconference = nbTriSurface >> 1;

	const float constanteRotation = 2 * glm::pi<float>() * rot;
	const float pasArcExt = constanteRotation / nbTranches;
	const float pasArc = 2 * glm::pi<float>() / nbPointCirconference;
	const float valeurNormDirFace = 1.0f / sqrt(constanteRotation * constanteRotation + L * L);

	glm::vec3* points = new glm::vec3[nbTranches * nbPointCirconference];
	glm::vec3* normales = new glm::vec3[nbTranches * nbPointCirconference];

	for (int i = 0; i < nbTranches; i++)
	{
		const float theta = i * pasArcExt;
		glm::vec3 face = glm::vec3(cos(theta), sin(theta), L * i / nbTranches);
		glm::vec3 dirFace = valeurNormDirFace * glm::vec3(constanteRotation * glm::vec2(-face.y, face.x), L);
		glm::vec3 dirFaceCote = glm::vec3(face.x, face.y, 0);
		glm::vec3 dirFaceHaut = glm::cross(dirFace, dirFaceCote);

		for (int j = 0; j < nbPointCirconference; j++)
		{
			const float phi = j * pasArc - ((i & 1) == 1 ? 0.5f * pasArc : 0.0f);
			const float rayon = r * cos(phi);

			glm::vec3 normale = dirFaceCote * cos(phi) + dirFaceHaut * sin(phi);
			//glm::vec3 point = glm::vec3(face.x * rayon + face.x * R, face.y * rayon + face.y * R, r * sin(phi) + face.z);
			glm::vec3 point = glm::vec3(R, R, 1) * face + r * normale;

			points[i * nbPointCirconference + j] = point;
			normales[i * nbPointCirconference + j] = normale;
		}
	}

	m.nbTriangle = (nbTranches - 1) * nbTriSurface + nbPointCirconference * 2;
	m.triangles = new Triangle[m.nbTriangle];
	for (int i = 0; i < nbTranches - 1; i++)
	{
		int i1 = i;
		int i2 = i + 1;
		i2 = i2 >= nbTranches ? 0 : i2;

		for (int j = 0; j < nbTriSurface; j += 2)
		{
			Triangle& tri1 = m.triangles[i * nbTriSurface + j];
			Triangle& tri2 = m.triangles[i * nbTriSurface + j + 1];

			int j11 = j >> 1;
			int j12 = j11 - 1;
			j12 += j12 < 0 ? nbPointCirconference : 0;
			int j21 = (i & 1) == 1 ? j12 : j11;
			int j22 = j21 - 1;
			j22 += j22 < 0 ? nbPointCirconference : 0;

			tri1.verts[2] = { points[i1 * nbPointCirconference + j11], glm::vec2(j11, i1), normales[i1 * nbPointCirconference + j11] };
			tri1.verts[1] = { points[i1 * nbPointCirconference + j12], glm::vec2(j12, i1), normales[i1 * nbPointCirconference + j12] };
			tri1.verts[0] = { points[i2 * nbPointCirconference + j21], glm::vec2(j21, i2), normales[i2 * nbPointCirconference + j21] };

			tri2.verts[0] = { points[i2 * nbPointCirconference + j21], glm::vec2(j21, i2), normales[i2 * nbPointCirconference + j21] };
			tri2.verts[1] = { points[i2 * nbPointCirconference + j22], glm::vec2(j22, i2), normales[i2 * nbPointCirconference + j22] };
			tri2.verts[2] = { points[i1 * nbPointCirconference + j12], glm::vec2(j12, i1), normales[i1 * nbPointCirconference + j12] };
		}
	}

	for (int i = 0; i < 2; i++)
	{
		bool deuxiemeFace = i == 1;

		const float angle = deuxiemeFace ? 2 * glm::pi<float>() * rot * (nbTranches - 1) / nbTranches : 0;
		glm::vec3 pointCentreFace = deuxiemeFace ? glm::vec3(R * cos(angle), R * sin(angle), L * (nbTranches - 1) / nbTranches) : glm::vec3(R, 0, 0);

		glm::vec3 dirFace = (deuxiemeFace ? -1 : 1) * valeurNormDirFace * glm::vec3(constanteRotation * glm::vec2(-pointCentreFace.y, pointCentreFace.x), L);

		uint32_t indexPoints = deuxiemeFace ? (nbTranches - 1) * nbPointCirconference : 0;

		uint32_t v0 = deuxiemeFace ? 1 : 0;
		uint32_t v1 = 1 - v0;

		for (int j = 0; j < nbPointCirconference; j++)
		{
			Triangle& tri1 = m.triangles[(nbTranches - 1) * nbTriSurface + j + (deuxiemeFace ? i * nbPointCirconference : 0)];

			int j1 = j;
			int j2 = j + 1;
			j2 = j2 >= nbPointCirconference ? 0 : j2;

			tri1.verts[v0] = { points[indexPoints + j1], glm::vec2(0, 0), -dirFace };
			tri1.verts[v1] = { points[indexPoints + j2], glm::vec2(0, 0), -dirFace };
			tri1.verts[2]  = { pointCentreFace,          glm::vec2(0, 0), -dirFace };
		}
	}

	delete[] points;
	delete[] normales;
}

void Application::initialiserMoteurGraphique(Application* const app)
{
	MoteurGX::init(&app->moteurGX);

	// Pipeline pour dessiner la vue en coupe

	{
		Ressource shaderIU, pipelineIU, vaoIU;
		Shader& shader = MoteurGX::creerShader(&app->moteurGX, &shaderIU);
		mgx::Pipeline& pipeline = MoteurGX::creerPipeline(&app->moteurGX, &pipelineIU);
		mgx::Pipeline::renduStandard(&pipeline, true);
		pipeline.tailleFenetre = glm::uvec2(app->fenetre.dimension.x, app->fenetre.dimension.y);
		pipeline.fbo = 0;
		pipeline.shader = shaderIU;
		pipeline.modeCulling = Operation::Culling::DESACTIVER;
		pipeline.equationMelange = Operation::Melange::ADDITION;

		{
			std::string shaderRaw;

			chargerFichier("Shaders/Simple/vertex.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::VERTEX);

			shaderRaw.clear();
			chargerFichier("Shaders/Simple/geometrie.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::GEOMETRIE);

			shaderRaw.clear();
			chargerFichier("Shaders/Simple/fragment.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::FRAGMENT);
		}
		Shader::assembler(shader);
		Shader::delier();
	}

	// Prefiltrage en Z (profondeur)
	// Optimisation pour ne faire les calculs d'éclairage que sur les pixels visibles

	{
		Ressource shaderIU, pipelineIU, vaoIU;
		Shader& shader = MoteurGX::creerShader(&app->moteurGX, &shaderIU);
		mgx::Pipeline& pipeline = MoteurGX::creerPipeline(&app->moteurGX, &pipelineIU);
		mgx::Pipeline::renduStandard(&pipeline, true);
		pipeline.tailleFenetre = glm::uvec2(app->fenetre.dimension.x, app->fenetre.dimension.y);
		pipeline.fbo = 0;
		pipeline.shader = shaderIU;
		pipeline.modeCulling = Operation::Culling::AVANT;
		pipeline.equationMelange = Operation::Melange::DESACTIVER;

		{
			std::string shaderRaw;

			chargerFichier("Shaders/PrefiltrageZ/vertex.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::VERTEX);

			shaderRaw.clear();
			chargerFichier("Shaders/PrefiltrageZ/fragment.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::FRAGMENT);
		}
		Shader::assembler(shader);
		Shader::delier();
	}

	// Pipeline pour dessiner le plan

	{
		Ressource shaderPlanIU, pipelinePlanIU;
		Shader& shaderPlan = MoteurGX::creerShader(&app->moteurGX, &shaderPlanIU);
		mgx::Pipeline& pipelinePlan = MoteurGX::creerPipeline(&app->moteurGX, &pipelinePlanIU);
		mgx::Pipeline::renduStandard(&pipelinePlan, false);
		pipelinePlan.tailleFenetre = glm::uvec2(app->fenetre.dimension.x, app->fenetre.dimension.y);
		pipelinePlan.fbo = 0;
		pipelinePlan.shader = shaderPlanIU;
		pipelinePlan.modeCulling = Operation::Culling::DESACTIVER;

		{
			std::string shaderRaw;

			chargerFichier("Shaders/Simple/vertex.glsl", &shaderRaw);
			Shader::loadSubShader(shaderPlan, shaderRaw, TypeShader::VERTEX);

			shaderRaw.clear();
			chargerFichier("Shaders/Simple/geometrie.glsl", &shaderRaw);
			Shader::loadSubShader(shaderPlan, shaderRaw, TypeShader::GEOMETRIE);

			shaderRaw.clear();
			chargerFichier("Shaders/Simple/fragmentPlan.glsl", &shaderRaw);
			Shader::loadSubShader(shaderPlan, shaderRaw, TypeShader::FRAGMENT);
		}
		Shader::assembler(shaderPlan);
		Shader::delier();
	}

	Vertex triangles[6];
	triangles[0] = { glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0) };
	triangles[1] = { glm::vec3(-1,  1, 0), glm::vec2(0, 1), glm::vec3(0, 0, 0) };
	triangles[2] = { glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::vec3(0, 0, 0) };
	triangles[3] = { glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::vec3(0, 0, 0) };
	triangles[4] = { glm::vec3( 1, -1, 0), glm::vec2(1, 0), glm::vec3(0, 0, 0) };
	triangles[5] = { glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0) };

	Model model;

	//decoderOBJ("C:\\Users\\Alexandre\\Desktop\\général francais\\etoile2.obj", &model);
	//decoderSTL("C:\\Users\\Alexandre\\Desktop\\Inventor proj\\Sous-marinV2\\C-Coque\\C-0015.stl", &model);
	
	mgx::Mesh::creer(&app->donnesOperation.meshSolenoide, &app->moteurGX);
	const float L = app->donnesOperation.solenoide.longueur;
	const float R = app->donnesOperation.solenoide.R;
	const float r = app->donnesOperation.solenoide.r;
	const float rot = app->donnesOperation.solenoide.nbRot;
	genererSolenoide(&model, 30000, R, r, L, rot);
	mgx::Mesh::chargerModel<Vertex>(&app->donnesOperation.meshSolenoide, &app->moteurGX, model.nbTriangle * 3, model.triangles);

	mgx::Mesh meshCarre;
	mgx::Mesh::creer(&meshCarre, &app->moteurGX);
	mgx::Mesh::chargerModel<Vertex>(&meshCarre, &app->moteurGX, 6, triangles);
	delete[] model.triangles;

	mgx::Mesh sphere;
	mgx::Mesh::creer(&sphere, &app->moteurGX);
	decoderOBJ("Mesh/Sphere.obj", &model);
	mgx::Mesh::chargerModel<Vertex>(&sphere, &app->moteurGX, model.nbTriangle * 3, model.triangles);
	delete[] model.triangles;

	int32_t skyboxX, skyboxY, skyboxCanaux;
	std::string dossier = "Skybox/Machine shop/1k/";
	std::string ext = ".hdr";
	std::string noms[6] = { "px", "nx", "py", "ny", "pz", "nz"};
	Texture& skybox = MoteurGX::creerTexture(&app->moteurGX, &app->donnesOperation.skyboxIU);
	Texture::allouerCubemap(&skybox, Tex::FormatInterne::RVB, Tex::Format::RVB, Donnee::Type::U8);
	stbi_set_flip_vertically_on_load(0);
	for (int i = 0; i < sizeof(noms) / sizeof(std::string); i++)
	{
		auto* pixels = stbi_load((dossier + noms[i] + ext).c_str(), &skyboxX, &skyboxY, &skyboxCanaux, 3);
		Texture::soumettreCubemap(&skybox, (Tex::FaceCubemap)((uint32_t)Tex::FaceCubemap::POSX + i), 0, glm::ivec2(0, 0), glm::ivec2(skyboxX, skyboxY), pixels);
		stbi_image_free(pixels);
	}
	Texture::specifierFiltre(skybox, Tex::Filtre::PROCHE, Tex::Filtre::PROCHE);
	Texture::specifierEtirement(skybox, Tex::Emballage::LIMITER_BORDURE, Tex::Emballage::LIMITER_BORDURE, Tex::Emballage::LIMITER_BORDURE);

	// Pipeline pour le skybox

	{
		Ressource shaderIU, pipelineIU, vaoIU;
		Shader& shader = MoteurGX::creerShader(&app->moteurGX, &shaderIU);
		mgx::Pipeline& pipeline = MoteurGX::creerPipeline(&app->moteurGX, &pipelineIU);
		mgx::Pipeline::renduStandard(&pipeline, false);
		pipeline.tailleFenetre = glm::uvec2(app->fenetre.dimension.x, app->fenetre.dimension.y);
		pipeline.fbo = 0;
		pipeline.shader = shaderIU;
		pipeline.modeCulling = Operation::Culling::AVANT;
		pipeline.testProfondeur = Operation::Profondeur::MOINS_EGAL;

		{
			std::string shaderRaw;

			chargerFichier("Shaders/Skybox/vertex.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::VERTEX);

			shaderRaw.clear();
			chargerFichier("Shaders/Skybox/fragment.glsl", &shaderRaw);
			Shader::loadSubShader(shader, shaderRaw, TypeShader::FRAGMENT);
		}
		Shader::assembler(shader);
		Shader::delier();
	}

	mgx::Mesh& cube = app->donnesOperation.meshCube;
	mgx::Mesh::creer(&cube, &app->moteurGX);
	decoderOBJ("Mesh/Cube.obj", &model);
	mgx::Mesh::chargerModel<Vertex>(&cube, &app->moteurGX, model.nbTriangle * 3, model.triangles);
	delete[] model.triangles;
}

void Application::initialiserInterfaceUtilisateur(Application* const app)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io->ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(app->fenetre.window, true);
	ImGui_ImplOpenGL3_Init("#version 450");

	//ImFont* dfont = io->Fonts->AddFontDefault();
	app->police = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\micross.ttf", 18.0f);

	// Style

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->TabRounding = 0.5f;

	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.392f, 0.0f, 0.64f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // When hover window title
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Active window in selection
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Window not active in selection
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.25f, 0.39f, 0.90f, 1.00f);
	style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TableBorderLight] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.43f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.80f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
}

glm::vec3 orientation(float yaw, float pitch) {
	float yawR = glm::radians(yaw);
	float pitchR = glm::radians(pitch);

	return glm::vec3(sin(yawR) * cos(pitchR), sin(pitchR), cos(yawR) * cos(pitchR));
}

void Application::initialiserSimulation(Application* const app)
{
	Ressource shaderIU, pipelineIU, vaoIU;
	Shader& shader = MoteurGX::creerShader(&app->moteurGX, &shaderIU);
	mgx::Pipeline& pipeline = MoteurGX::creerPipeline(&app->moteurGX, &pipelineIU);
	mgx::Pipeline::renduStandard(&pipeline, true);
	pipeline.fbo = 0;
	pipeline.shader = shaderIU;
	pipeline.tailleFenetre = glm::uvec2(app->fenetre.dimension.x, app->fenetre.dimension.y);

	{
		std::string shaderRaw;

		chargerFichier("Shader/Vertex.glsl", &shaderRaw);
		Shader::loadSubShader(shader, shaderRaw, TypeShader::VERTEX);
		shaderRaw.clear();

		chargerFichier("Shader/Geometry.glsl", &shaderRaw);
		Shader::loadSubShader(shader, shaderRaw, TypeShader::GEOMETRIE);
		shaderRaw.clear();

		chargerFichier("Shader/Fragment.glsl", &shaderRaw);
		Shader::loadSubShader(shader, shaderRaw, TypeShader::FRAGMENT);
	}

	Shader::assembler(shader);
	Shader::delier();

	MoteurGX::Ressource ressource;

	Vertexarray& vao = MoteurGX::creerVertexarray(&app->moteurGX, &ressource);
	vao.nbTriangles = 1;
}

void Application::executerEntrees(Application* const app, const float dt)
{
	GLFWwindow* fenetre = app->fenetre.window;
	float& tangageCam = app->donnesOperation.tangageCam;
	float& lacetCam = app->donnesOperation.lacetCam;
	float& distanceCam = app->donnesOperation.distanceCam;

	glm::vec3& camPos = app->donnesOperation.camPos;

	lacetCam += (glfwGetKey(fenetre, GLFW_KEY_RIGHT) - glfwGetKey(fenetre, GLFW_KEY_LEFT)) * dt * 100;
	tangageCam += (glfwGetKey(fenetre, GLFW_KEY_UP) - glfwGetKey(fenetre, GLFW_KEY_DOWN)) * dt * 100;
	distanceCam += (glfwGetKey(fenetre, GLFW_KEY_L) - glfwGetKey(fenetre, GLFW_KEY_O)) * dt * 30;

	glm::vec3 dirCam = orientation(lacetCam, tangageCam);
	glm::vec3 coteCam = orientation(lacetCam - 90.0f, 0.0f);
	glm::vec3 hautCam = glm::cross(dirCam, coteCam);

	camPos += dirCam * (float)((glfwGetKey(fenetre, GLFW_KEY_W) - glfwGetKey(fenetre, GLFW_KEY_S)) * dt * 30.0f);
	camPos += coteCam * (float)((glfwGetKey(fenetre, GLFW_KEY_A) - glfwGetKey(fenetre, GLFW_KEY_D)) * dt * 30.0f);
	camPos -= hautCam * (float)((glfwGetKey(fenetre, GLFW_KEY_R) - glfwGetKey(fenetre, GLFW_KEY_F)) * dt * 30.0f);
	
}

void Application::executerRendu(Application* const app)
{
	GLFWwindow* fenetre = app->fenetre.window;
	const float tangageCam = app->donnesOperation.tangageCam;
	const float lacetCam = app->donnesOperation.lacetCam;
	const float distanceCam = app->donnesOperation.distanceCam;
	const glm::vec2 angleLumiere = app->donnesOperation.angleLumiere;

	const float projFOV = 70.0f;
	const float aspectRatio = 1.5f;

	glm::vec3 camDir = orientation(lacetCam, tangageCam) * glm::vec3(1, 1, 1);
	glm::vec3 camPos = app->donnesOperation.camPos;

	glm::mat4 proj = glm::perspective(projFOV, aspectRatio, 1.0f, -1.0f);
	glm::mat4 rotationCam = glm::lookAt(glm::vec3(0.0f), camDir, glm::vec3(0, 1, 0));
	glm::mat4 vueCam = proj * rotationCam * glm::translate(glm::mat4(1.0f), -app->donnesOperation.camPos);

	glm::vec3 posLumiere = orientation(angleLumiere.x, angleLumiere.y);

	{
		//const Shader& shader = MoteurGX::demarerCouche(app->moteurGX, 1);
		//Shader::pousserConstanteMat4(shader, "u_cam", vueCam);
		//MoteurGX::executerCouche(app->moteurGX, 0);
	}

	// Dessiner l'objet

	{
		const Shader& shader = MoteurGX::demarerCouche(app->moteurGX, 0);
		Shader::pousserConstanteMat4(shader, "u_cam", vueCam);
		Shader::pousserConstanteVec3(shader, "u_couleur", app->donnesOperation.couleur);
		Shader::pousserConstanteVec3(shader, "u_dirLumiere", posLumiere);
		Shader::pousserConstanteVec3(shader, "u_posCam", camPos);
		Shader::pousserConstanteVirgule(shader, "u_disZ", app->donnesOperation.disZ);
		Shader::pousserConstanteVirgule(shader, "u_ka", app->donnesOperation.kAmbient);
		Shader::pousserConstanteVirgule(shader, "u_ks", app->donnesOperation.kSpeculaire);
		Shader::pousserConstanteVirgule(shader, "u_kd", app->donnesOperation.kDiffuse);
		Shader::pousserConstanteVirgule(shader, "u_exposantSpec", app->donnesOperation.esposantSpec);
		Shader::pousserTexture(shader, "u_skybox", MoteurGX::retTexture(app->moteurGX, 0), 0);
		MoteurGX::executerCouche(app->moteurGX, 0);
	}

	// Dessiner le skybox

	{
		glm::mat4 matrice = rotationCam;

		const Shader& shader = MoteurGX::demarerCouche(app->moteurGX, 3);
		Texture::lier(MoteurGX::retTexture(app->moteurGX, app->donnesOperation.skyboxIU));
		Shader::pousserTexture(shader, "u_skybox", MoteurGX::retTexture(app->moteurGX, 0), 0);
		for (int i = 0; i < 1; i++)
		{
			Shader::pousserConstanteMat4(shader, "u_cam", proj * matrice);
			MoteurGX::executerCouche(app->moteurGX, 3);

			matrice *= rotationCam;
		}
	}

	// Dessiner le plan

	glm::mat4 matriceModel = glm::mat4(
		0, 0, 10, 0,
		0, 10, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 1
		);

	glm::mat4 matricePlan = glm::translate(glm::mat4(1.0f), glm::vec3(app->donnesOperation.disZ, 0, 5)) * matriceModel;

	const Shader& shaderPlan = MoteurGX::demarerCouche(app->moteurGX, 2);
	Shader::pousserConstanteVec3(shaderPlan, "u_dirLumiere", posLumiere);
	Shader::pousserConstanteVec3(shaderPlan, "u_posCam", camPos);

	Shader::pousserConstanteMat4(shaderPlan, "u_cam", vueCam * glm::translate(glm::mat4(1.0f), 20.0f * posLumiere));
	Shader::pousserConstanteVec3(shaderPlan, "u_couleur", glm::vec3(1, 1, 1));
	MoteurGX::executerCouche(app->moteurGX, 2);

	Shader::pousserConstanteMat4(shaderPlan, "u_cam", vueCam * matricePlan);
	Shader::pousserConstanteVec3(shaderPlan, "u_couleur", glm::vec3(0, 1, 1));
	MoteurGX::executerCouche(app->moteurGX, 1);

	//MoteurGX::copierRenduBackbuffer(app->moteurGX, glm::uvec2(800, 600));
}

void Application::executerSimulation(Application* const app)
{
	auto tAvant = std::chrono::high_resolution_clock::now();

	MoteurPhysique moteurPhysique;
	Espace::GPU::initialiser(&moteurPhysique.coordonnees, glm::ivec3(100, 100, 100));
	Espace::GPU::initialiser(&moteurPhysique.champMagnetique, glm::ivec3(100, 100, 100));

	MoteurPhysique::Info info;
	info.fils.push_back(MoteurPhysique::Fil::creer(glm::vec3(0, 0, 1), glm::vec3(-1.0f, -1.0f, 0.0f), -3.0f));
	info.fils.push_back(MoteurPhysique::Fil::creer(glm::vec3(0, 0, 1), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f));
	info.fils.push_back(MoteurPhysique::Fil::creer(glm::vec3(0, 0, -1), glm::vec3(0.0f, 1.5f, 0.0f), 1.0f));

	MoteurPhysique::GPU::genererBufferInfo(&info);
	MoteurPhysique::GPU::soumettreBufferInfo(info);

	MoteurPhysique::GPU::chargerShaders(&moteurPhysique, "Shader/");
	MoteurPhysique::GPU::assignerCoordonnees(moteurPhysique, glm::vec3(-2), glm::vec3(2));

	Texture gradient;
	{
		unsigned int contenu_gradient[] = { 0x00ff0002, 0x00fd7405, 0x00fbe701, 0x00dfff01, 0x004efd03, 0x0008fe49, 0x0003fde4, 0x0000e3ff, 0x000071fe, 0x000b00ff };
		Texture::generer(&gradient);
		Texture::specifierEtirement(gradient, Tex::Emballage::LIMITER_BORD, Tex::Emballage::LIMITER_BORD, Tex::Emballage::LIMITER_BORD);
		Texture::specifierFiltre(gradient, Tex::Filtre::LINEAIRE, Tex::Filtre::LINEAIRE);
		Texture::allouer1D(&gradient, 0, glm::ivec1(10), Tex::FormatInterne::RVBA, Tex::Format::RVBA, Donnee::Type::U8, contenu_gradient);
	}

	Camera camera = {};

	GLuint vba;
	Ressource vaoIU, vboIU;
	APPEL_GX(glGenVertexArrays(1, &vba));
	Vertexarray& vao = MoteurGX::creerVertexarray(&app->moteurGX, &vaoIU);
	Vertexarray::lier(vao);
	Vertexbuffer& vbo = MoteurGX::creerVertexbuffer(&app->moteurGX, &vboIU);
	Vertexbuffer::allocation(&vbo, 10);
	Vertexarray::ajouterAttribut(vao, vbo, 0, 1, Donnee::Type::VIRGULE, false, 1, 0);

	while (!glfwWindowShouldClose(app->fenetre.window))
	{
		auto tMaintenant = std::chrono::high_resolution_clock::now();
		const float dt = (tMaintenant - tAvant).count() / 1000000000.0f;
		tAvant = tMaintenant;

		MoteurPhysique::GPU::executerCalcul(moteurPhysique.shaderChampMagnetique, moteurPhysique.coordonnees, moteurPhysique.champMagnetique, info);

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		Shader& shader = MoteurGX::retShader(app->moteurGX, MoteurGX::retPipeline(app->moteurGX, 0).shader);
		APPEL_GX(glUseProgram(shader.id));
		APPEL_GX(glBindVertexArray(vba));

		Camera::input(&camera, &app->fenetre, dt);
		Camera::transformer(&camera);
		glm::ivec2 nombreFleches = glm::ivec2(50);
		Shader::pousserConstanteIVec2(shader, "dimension", nombreFleches);
		Shader::pousserConstanteMat4(shader, "transformation", camera.plan);
		Shader::pousserTexture(shader, "carte", moteurPhysique.champMagnetique.tex, 0);
		Shader::pousserTexture(shader, "gradient", gradient, 1);

		APPEL_GX(glDrawArrays(GL_POINTS, 0, nombreFleches.x * nombreFleches.y));

		//Application::executerSimulation(app, dt);

		glfwSwapBuffers(app->fenetre.window);
		glfwPollEvents();
	}
}