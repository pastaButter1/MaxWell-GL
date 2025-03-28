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

using Ressource = MoteurGX::Ressource;

void Application::initialiser(Application* const app, glm::uvec2 tailleFenetre)
{
	Fenetre::init(&app->fenetre, tailleFenetre);

	initialiserInterfaceUtilisateur(app);

	initaliserMoteurGraphique(app);
}

void Application::executer(const Application& app)
{
	auto tAvant = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(app.fenetre.window))
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

		ImGui::Begin("fenetre");

		static glm::vec4 couleur;

		ImGui::ColorEdit4("Couleur", (float*)&couleur);

		ImGui::End();

		ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		MoteurGX::demarerCouche(app.moteurGX, 0);
		Vertexarray::lier(MoteurGX::retVertexarray(app.moteurGX, 0));

		MoteurGX::executerCouche(app.moteurGX);

		glfwSwapBuffers(app.fenetre.window);
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
	pipeline.testProfondeur = GL_ALWAYS;
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

	decoderOBJ("C:\\Users\\Alexandre\\Desktop\\général francais\\etoile2.obj", &model);

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
