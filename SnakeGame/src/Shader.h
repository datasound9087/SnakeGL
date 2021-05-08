#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

class Shader
{
public:
	Shader(const std::string& vShader, const std::string& fShader);
	~Shader();

	void use();

	void setMat4(const std::string& name, const glm::mat4& mat);
	void setVec3f(const std::string& name, const glm::vec3& vec);

private:

	void checkCompileErrors(unsigned int shader, std::string type);

	GLuint mId;
};

