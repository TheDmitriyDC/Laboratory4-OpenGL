#include "shader.h"

#include <iostream>

CShader & CShader::Use()
{
	glUseProgram(this->Program);
	return *this;
}

void CShader::Compile(const GLchar * vertexSource, const GLchar * fragmentSource, const GLchar * geometrySource)
{
	GLuint sVertex, sFragment, gShader;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// If geometry shader source code is given, also compile geometry shader
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, sVertex);
	glAttachShader(this->Program, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(this->Program, gShader);
	glLinkProgram(this->Program);
	checkCompileErrors(this->Program, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void CShader::CompileTess(const GLchar * vertexSource, const GLchar * fragmentSource, const GLchar * geometrySource,
	const GLchar * TessControlSource, const GLchar * TessEvalSource)
{
	GLuint sVertex, sFragment, gShader, tessControl, tessEval;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	//geometry Shader
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	// tessControl Shader
	tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tessControl, 1, &TessControlSource, NULL);
	glCompileShader(tessControl);
	checkCompileErrors(tessControl, "TESSCONTROL");
	// tessEval Shader
	tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tessEval, 1, &TessEvalSource, NULL);
	glCompileShader(tessEval);
	checkCompileErrors(tessEval, "TESSEVAL");

	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, sVertex);
	glAttachShader(this->Program, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(this->Program, gShader);
	glAttachShader(this->Program, tessControl);
	glAttachShader(this->Program, tessEval);

	glLinkProgram(this->Program);
	checkCompileErrors(this->Program, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
	glDeleteShader(tessControl);
	glDeleteShader(tessEval);
}

void CShader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->Program, name), value);
}
void CShader::SetInteger(const GLchar *name, GLint value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->Program, name), value);
}
void CShader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->Program, name), x, y);
}
void CShader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->Program, name), value.x, value.y);
}
void CShader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->Program, name), x, y, z);
}
void CShader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->Program, name), value.x, value.y, value.z);
}
void CShader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->Program, name), x, y, z, w);
}
void CShader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->Program, name), value.x, value.y, value.z, value.w);
}
void CShader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->Program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}


void CShader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}