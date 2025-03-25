#include "Souris.h"

#include "Lib/GLFW/include/GLFW/glfw3.h"
#include "Application/Fenetre/Fenetre.h"

void Souris::callbackBouton(GLFWwindow* window, int bouton, int action, int modes)
{
	Fenetre* fenetre = Fenetre::ret(window);

	if (bouton == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &fenetre->souris.position.x, &fenetre->souris.position.y);
	}
}