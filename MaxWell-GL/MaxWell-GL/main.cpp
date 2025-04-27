#include "Application/Application.h"
#include "Application/Utilitaire.h"

int main() {
	Application application;

	Application::initialiser(&application, glm::uvec2(1200, 800));
	
	Application::executer(&application);
	
	Application::fermer(&application);

	return 0;
}