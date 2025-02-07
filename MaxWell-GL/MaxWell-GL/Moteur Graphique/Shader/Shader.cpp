#include <string>

#include "Shader.h"

void Shader::generer(Shader* const shader)
{
	shader->id = APPEL_GX(glCreateProgram());
}

void Shader::detruire(const Shader shader)
{
	APPEL_GX(glDeleteShader(shader.id));
}

void Shader::loadSubShader(const Shader& shader, const std::string& fileData, uint32_t type)
{
	APPEL_GX(uint32_t id = glCreateShader(type));

	const char* donnee = fileData.c_str();

	APPEL_GX(glShaderSource(id, 1, &donnee, NULL));

	APPEL_GX(glCompileShader(id));

	int32_t result;

	APPEL_GX(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int32_t length = 1024;
		APPEL_GX(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* infoLog = (char*)alloca(length * sizeof(char));
		APPEL_GX(glGetShaderInfoLog(id, length, &length, infoLog));

		printf("ERREUR | Probleme lors de la compilation du shader : %s\n", infoLog);

		APPEL_GX(glDeleteShader(id));

		return;
	}

	APPEL_GX(glAttachShader(shader.id, id));
	glDeleteShader(id);
}

void Shader::build(const Shader& shader)
{
	APPEL_GX(glLinkProgram(shader.id));
	//glValidateProgram(shader.id);

	int32_t result;
	char infoLog[512];

	APPEL_GX(glGetProgramiv(shader.id, GL_LINK_STATUS, &result));
	if (result == GL_FALSE) {
		APPEL_GX(glGetProgramInfoLog(shader.id, 512, NULL, infoLog));

		printf("ERREUR | Probleme lors de la liaison du shader : %s\n", infoLog);
	}
}

void Shader::lier(const Shader& shader)
{
	APPEL_GX(glUseProgram(shader.id));
}

void Shader::delier()
{
	APPEL_GX(glUseProgram(0));
}

void Shader::pushConstantMat4(const Shader& shader, const std::string& name, const glm::mat4& matrix)
{
	uint32_t loc = APPEL_GX(glGetUniformLocation(shader.id, name.c_str()));
	APPEL_GX(glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)&matrix));
}

void Shader::pushConstantMat3(const Shader& shader, const std::string& name, const glm::mat3& matrix)
{
	uint32_t loc = APPEL_GX(glGetUniformLocation(shader.id, name.c_str()));
	APPEL_GX(glUniformMatrix3fv(loc, 1, GL_TRUE, (float*)&matrix));
}

void Shader::pushConstantVec3(const Shader& shader, const std::string& name, glm::vec3 v)
{
	uint32_t loc = APPEL_GX(glGetUniformLocation(shader.id, name.c_str()));
	APPEL_GX(glUniform3f(loc, v.x, v.y, v.z));
}

void Shader::pushConstantVec2(const Shader& shader, const std::string& name, glm::vec2 v)
{
	uint32_t loc = APPEL_GX(glGetUniformLocation(shader.id, name.c_str()));
	APPEL_GX(glUniform2f(loc, v.x, v.y));
}

void Shader::pushConstantFloat(const Shader& shader, const std::string& name, float f)
{
	uint32_t loc = APPEL_GX(glGetUniformLocation(shader.id, name.c_str()));
	APPEL_GX(glUniform1f(loc, f))
}

void Shader::pushTexture(const Shader& shader, const std::string& name, const uint32_t texSlot)
{
	uint32_t loc = APPEL_GX(glGetUniformLocation(shader.id, name.c_str()));
	APPEL_GX(glUniform1i(loc, texSlot));
}
