#include "Application.h"
#include "Moteur Graphique/Graphique.h"
#include "Lib/GLFW/include/GLFW/glfw3.h"

#include "Lib/IMGUI/IMGUI/imgui.h"
#include "Lib/IMGUI/IMGUI/imgui_impl_glfw.h"
#include "Lib/IMGUI/IMGUI/imgui_impl_opengl3.h"

#include "Utilitaire.h"

#include <conio.h>
#include <chrono>

#include "Moteur Graphique/Model/DecoderFichier.h"
#include "Moteur Graphique/Vertexbuffer/Vertexbuffer.h"
#include "Moteur Graphique/MoteurGx/Mesh.h"

#include "Lib/GLM/glm/matrix.hpp"
#include "Lib/GLM/glm/gtc/matrix_transform.hpp"

using Ressource = MoteurGX::Ressource;

void Application::initialiser(Application* const app, glm::uvec2 tailleFenetre)
{
	Fenetre::init(&app->fenetre, tailleFenetre);

	initialiserInterfaceUtilisateur(app);

	initaliserMoteurGraphique(app);
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

		ImGui::End();

		ImGui::Begin("fenetre");

		static glm::vec4 couleur;

		//ImGui::ColorEdit4("Couleur", (float*)&couleur);
		ImGui::Image((void*)MoteurGX::retTexture(app->moteurGX, 0).id, ImVec2(800, 600), ImVec2(1, 1), ImVec2(0, 0));

		ImGui::End();

		executerEntrees(app, dt);
		executerRendu(app);
		//MoteurGX::copierRenduBackbuffer(app->moteurGX, glm::uvec2(800, 600));

		APPEL_GX(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		
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

void genererSolenoide(Model* const model, const uint32_t nbTriangles, const float R, const float r, const float l, const float rot)
{
	Model& m = *model;

	const uint32_t nbTranches = rot * sqrt(nbTriangles);
	const uint32_t nbTriSurface = nbTriangles / nbTranches;
	const uint32_t nbPointCirconference = nbTriSurface >> 1;

	const float pasArcExt = rot * 2 * glm::pi<float>() / nbTranches;
	const float pasArc = 2 * glm::pi<float>() / nbPointCirconference;

	glm::vec3* points = new glm::vec3[nbTranches * nbPointCirconference];

	for (int i = 0; i < nbTranches; i++)
	{
		const float theta = i * pasArcExt;
		glm::vec3 face = glm::vec3(cos(theta), sin(theta), l * i / nbTranches);

		for (int j = 0; j < nbPointCirconference; j++)
		{
			const float phi = j * pasArc - ((i & 1) == 1 ? 0.5f * pasArc : 0.0f);
			const float rayon = r * cos(phi);
			glm::vec3 point = glm::vec3(face.x * rayon + face.x * R, face.y * rayon + face.y * R, r * sin(phi) + face.z);

			points[i * (nbPointCirconference)+j] = point;
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

			tri1.verts[0] = { points[i1 * nbPointCirconference + j11], glm::vec2(j11, i1), glm::vec3(0.0f) };
			tri1.verts[1] = { points[i1 * nbPointCirconference + j12], glm::vec2(j12, i1), glm::vec3(0.0f) };
			tri1.verts[2] = { points[i2 * nbPointCirconference + j21], glm::vec2(j21, i2), glm::vec3(0.0f) };

			tri2.verts[2] = { points[i2 * nbPointCirconference + j21], glm::vec2(j21, i2), glm::vec3(0.0f) };
			tri2.verts[1] = { points[i2 * nbPointCirconference + j22], glm::vec2(j22, i2), glm::vec3(0.0f) };
			tri2.verts[0] = { points[i1 * nbPointCirconference + j12], glm::vec2(j12, i1), glm::vec3(0.0f) };
		}
	}

	for (int i = 0; i < 2; i++)
	{
		bool deuxiemeFace = i == 1;

		const float angle = deuxiemeFace ? 2 * glm::pi<float>() * rot * (nbTranches - 1) / nbTranches : 0;
		glm::vec3 pointCentreFace = deuxiemeFace ? glm::vec3(R * cos(angle), R * sin(angle), l * (nbTranches - 1) / nbTranches) : glm::vec3(R, 0, 0);

		uint32_t indexPoints = deuxiemeFace ? (nbTranches - 1) * nbPointCirconference : 0;

		uint32_t v0 = deuxiemeFace ? 1 : 0;
		uint32_t v1 = 1 - v0;

		for (int j = 0; j < nbPointCirconference; j++)
		{
			Triangle& tri1 = m.triangles[(nbTranches - 1) * nbTriSurface + j + (deuxiemeFace ? i * nbPointCirconference : 0)];

			int j1 = j;
			int j2 = j + 1;
			j2 = j2 >= nbPointCirconference ? 0 : j2;

			tri1.verts[v0] = { points[indexPoints + j1], glm::vec2(0, 0), glm::vec3(0.0f) };
			tri1.verts[v1] = { points[indexPoints + j2], glm::vec2(0, 0), glm::vec3(0.0f) };
			tri1.verts[2]  = { pointCentreFace,          glm::vec2(0, 0), glm::vec3(0.0f) };
		}
	}

	delete[] points;
}

void Application::initaliserMoteurGraphique(Application* const app)
{
	MoteurGX::init(&app->moteurGX);

	Ressource shaderIU, pipelineIU, vaoIU;
	Shader& shader = MoteurGX::creerShader(&app->moteurGX, &shaderIU);
	mgx::Pipeline& pipeline = MoteurGX::creerPipeline(&app->moteurGX, &pipelineIU);
	pipeline.fbo = 0;
	pipeline.shader = shaderIU;
	pipeline.modeDessin = GL_TRIANGLES;
	pipeline.equationMelange = GL_FUNC_ADD;
	pipeline.modeMelangeSRC = GL_SRC_ALPHA;
	pipeline.modeMelangeDST = GL_ONE_MINUS_SRC_ALPHA;
	pipeline.testProfondeur = GL_LESS;
	pipeline.modeEliminationFace = GL_BACK;
	pipeline.tailleFenetre = glm::uvec2(app->fenetre.dimension.x, app->fenetre.dimension.y);
	pipeline.stencilFunc = FUNC_TOUJOURS;
	pipeline.stencilMasque = 0xFF;
	pipeline.stencilRef = 0xFF;
	pipeline.stencilEchec = STENCIL_FUNC_GARDER;
	pipeline.profondeurEchec = STENCIL_FUNC_GARDER;
	pipeline.stencilProfondeurReussite = STENCIL_FUNC_GARDER;

	{
		std::string shaderRaw;

		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Simple\\vertex.glsl", &shaderRaw);
		Shader::loadSubShader(shader, shaderRaw, SHADER_VERTEX);

		shaderRaw.clear();
		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Simple\\geometrie.glsl", &shaderRaw);
		Shader::loadSubShader(shader, shaderRaw, SHADER_GEOMETRIE);

		shaderRaw.clear();
		chargerFichier("C:\\Users\\Alexandre\\Desktop\\OpenGLRoot\\MaxWell-GL\\MaxWell-GL\\MaxWell-GL\\Shaders\\Simple\\fragment.glsl", &shaderRaw);
		Shader::loadSubShader(shader, shaderRaw, SHADER_FRAGMENT);
	}

	Shader::assembler(shader);
	Shader::delier();

	/*Vertexarray& vao = MoteurGX::creerVertexarray(&app->moteurGX, &vaoIU);
	Vertexbuffer vbo;
	Vertexbuffer::generer(&vbo);
	Vertexbuffer::allocation(&vbo, 6 * sizeof(glm::vec2));
	vao.nbTriangles = 2;*/
	/*Vertex triangles[6];
	triangles[0] = { glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0) };
	triangles[1] = { glm::vec3(-1,  1, 0), glm::vec2(0, 1), glm::vec3(0, 0, 0) };
	triangles[2] = { glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::vec3(0, 0, 0) };
	triangles[3] = { glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::vec3(0, 0, 0) };
	triangles[4] = { glm::vec3( 1, -1, 0), glm::vec2(1, 0), glm::vec3(0, 0, 0) };
	triangles[5] = { glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::vec3(0, 0, 0) };*/

	Model model;

	//decoderOBJ("C:\\Users\\Alexandre\\Desktop\\général francais\\etoile2.obj", &model);
	//decoderSTL("C:\\Users\\Alexandre\\Desktop\\Inventor proj\\Sous-marinV2\\C-Coque\\C-0015.stl", &model);
	
	genererSolenoide(&model, 30000, 5, 0.5, 10, 6);

	mgx::Mesh mesh;
	mgx::Mesh::creer(&mesh, &app->moteurGX);
	mgx::Mesh::chargerModel<Vertex>(&mesh, &app->moteurGX, model.nbTriangle * 3, model.triangles);
	
	delete[] model.triangles;
	
	/*Vertexbuffer::transfererDonnees(vbo, 0, 6 * sizeof(glm::vec2), triangles);

	Vertexarray::ajouterAttribut(vao,vbo, 0, 2, TYPE_VIRGULE, TYPE_FAUX, sizeof(glm::vec2), 0);
	Vertexarray::delier();
	Vertexbuffer::delier();*/
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

void Application::executerEntrees(Application* const app, const float dt)
{
	GLFWwindow* fenetre = app->fenetre.window;
	float& tangageCam = app->donnesOperation.tangageCam;
	float& lacetCam = app->donnesOperation.lacetCam;
	float& distanceCam = app->donnesOperation.distanceCam;

	lacetCam += (glfwGetKey(fenetre, GLFW_KEY_LEFT) - glfwGetKey(fenetre, GLFW_KEY_RIGHT)) * dt * 50;
	tangageCam += (glfwGetKey(fenetre, GLFW_KEY_UP) - glfwGetKey(fenetre, GLFW_KEY_DOWN)) * dt * 50;
	distanceCam += (glfwGetKey(fenetre, GLFW_KEY_L) - glfwGetKey(fenetre, GLFW_KEY_O)) * dt * 30;
}

glm::vec3 orientation(float yaw, float pitch) {
	float yawR = glm::radians(yaw);
	float pitchR = glm::radians(pitch);

	return glm::vec3(sin(yawR) * cos(pitchR), sin(pitchR), cos(yawR) * cos(pitchR));
}

void Application::executerRendu(Application* const app)
{
	const Shader& shader = MoteurGX::demarerCouche(app->moteurGX, 0);
	Vertexarray::lier(MoteurGX::retVertexarray(app->moteurGX, 0));
	//MoteurGX::retShader(app->moteurGX, MoteurGX::retPipeline(app->moteurGX, 0).shader);

	GLFWwindow* fenetre = app->fenetre.window;
	const float tangageCam = app->donnesOperation.tangageCam;
	const float lacetCam = app->donnesOperation.lacetCam;
	const float distanceCam = app->donnesOperation.distanceCam;

	const float projFOV = 70.0f;
	const float aspectRatio = 1.5f;

	glm::vec3 camPos = orientation(lacetCam, tangageCam) * distanceCam;

	glm::mat4 proj = glm::perspective(projFOV, aspectRatio, 1.0f, -1.0f);
	glm::mat4 rotationCam = glm::lookAt(camPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));
	glm::mat4 vueCam = proj * rotationCam;

	Shader::pousserConstanteMat4(shader, "u_cam", vueCam);
	Shader::pousserConstanteVec3(shader, "u_couleur", glm::vec3(1, 0, 1));
	Shader::pousserConstanteVec3(shader, "u_dirLumiere", orientation(45, 45));
	Shader::pousserConstanteVec3(shader, "u_posCam", camPos);
	Shader::pousserConstanteVirgule(shader, "u_disZ", app->donnesOperation.disZ);

	MoteurGX::executerCouche(app->moteurGX);

	//MoteurGX::copierRenduBackbuffer(app->moteurGX, glm::uvec2(800, 600));
}