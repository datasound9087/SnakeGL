#include "Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vShader, const std::string& fShader)
{
	std::string vShaderCode;
	std::string fShaderCode;

	std::ifstream vFile;
	std::ifstream fFile;

	vFile.exceptions(std::ifstream::failbit);
	fFile.exceptions(std::ifstream::failbit);

	try
	{
		vFile.open(vShader);
		fFile.open(fShader);

		std::stringstream v, f;
		v << vFile.rdbuf();
		f << fFile.rdbuf();

		vFile.close();
		fFile.close();

		vShaderCode = v.str();
		fShaderCode = f.str();
	}
	catch (std::ifstream::failure fail)
	{
		throw std::runtime_error("ERROR READING SHADER FILES");
	}

	const char* v = vShaderCode.c_str();
	const char* f = fShaderCode.c_str();

	GLuint vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	GLuint frag;
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &f, NULL);
	glCompileShader(frag);
	checkCompileErrors(frag, "FRAGMENT");

	mId = glCreateProgram();
	glAttachShader(mId, vertex);
	glAttachShader(mId, frag);
	glLinkProgram(mId);
	checkCompileErrors(mId, "PROGRAM");

	glDeleteShader(frag);
	glDeleteShader(vertex);
}


Shader::~Shader()
{
	glDeleteProgram(mId);
}

void Shader::use()
{
	glUseProgram(mId);
}

void Shader::setMat4(const std::string& name, const glm::mat4 & mat)
{
	GLuint loc = glGetUniformLocation(mId, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3f(const std::string & name, const glm::vec3 & vec)
{
	GLuint loc = glGetUniformLocation(mId, name.c_str());
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{

	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
