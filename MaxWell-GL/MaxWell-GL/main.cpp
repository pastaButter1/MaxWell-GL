#include "Application/Application.h"
#include "Application/Utilitaire.h"

int main() {
	Application application;

	Application::initialiser(&application, glm::uvec2(800, 600));
	
	Application::executer(&application);
	
	Application::fermer(&application);

	return 0;
}