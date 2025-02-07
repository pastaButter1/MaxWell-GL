#pragma once

#include "Moteur Graphique/Graphique.h"

#include "Moteur Graphique/Texture/Texture.h"

#include <string>

struct Shader
{
	int32_t id = -1;

	static void generer(Shader *const shader);

	static void detruire(const Shader shader);

	static void loadSubShader(const Shader& shader, const std::string& fileData, uint32_t type);

	static void build(const Shader& shader);

	static void lier(const Shader& shader);

	static void delier();

	static void pushConstantMat4(const Shader& shader, const std::string& name, const glm::mat4& matrix);

	static void pushConstantMat3(const Shader& shader, const std::string& name, const glm::mat3& matrix);

	static void pushConstantVec3(const Shader& shader, const std::string& name, glm::vec3 v);

	static void pushConstantVec2(const Shader& shader, const std::string& name, glm::vec2 v);

	static void pushConstantFloat(const Shader& shader, const std::string& name, float f);

	static void pushTexture(const Shader& shader, const std::string& name, const uint32_t texSlot);
};

