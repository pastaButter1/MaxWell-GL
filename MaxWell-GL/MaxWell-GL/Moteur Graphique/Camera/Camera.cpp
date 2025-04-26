#include "Camera.h"

void Camera::input(Camera* const camera, Fenetre* const fenetre, float deltaTemps)
{
	if (glfwGetMouseButton(fenetre->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		camera->rotation += glm::vec2(fenetre->souris.deplacement) / glm::vec2(fenetre->dimension) * glm::vec2(1.0f);
	}

	if (glfwGetKey(fenetre->window, GLFW_KEY_UP) == GLFW_TRUE)
		camera->position += glm::vec3(0.0f, 0.0f, 0.5f) * glm::vec3(deltaTemps);
	if (glfwGetKey(fenetre->window, GLFW_KEY_DOWN) == GLFW_TRUE)
		camera->position -= glm::vec3(0.0f, 0.0f, 0.5f) * glm::vec3(deltaTemps);
}

void Camera::transformer(Camera* const camera)
{
	glm::mat4 plan = glm::mat4(1.0f);

	plan = glm::translate(plan, camera->position);
	plan = glm::rotate(plan, camera->rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));

	camera->plan = plan;
}