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

	static void assembler(const Shader& shader);

	static void lier(const Shader& shader);

	static void delier();

	static void pousserConstanteMat4(const Shader& shader, const std::string& name, const glm::mat4& matrix);

	static void pousserConstanteMat3(const Shader& shader, const std::string& name, const glm::mat3& matrix);

	static void pousserConstanteVec3(const Shader& shader, const std::string& name, glm::vec3 v);

	static void pousserConstanteVec2(const Shader& shader, const std::string& name, glm::vec2 v);

	static void pousserConstanteUVec2(const Shader& shader, const std::string& name, glm::uvec2 v);

	static void pousserConstanteVirgule(const Shader& shader, const std::string& name, float f);

	static void pousserTexture(const Shader& shader, const std::string& name, const uint32_t texSlot);
};

