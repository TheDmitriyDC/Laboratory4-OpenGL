#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CShader
{
public:
    GLuint Program;

	CShader() { }
	// Sets the current shader as active
	CShader  &Use();
	// Compiles the shader from given source code
	void    Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr); // Note: geometry source code is optional 
	void    CompileTess(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr, const GLchar *TessControlSource = nullptr, const GLchar *TessEvalSource = nullptr); // Note: geometry source code is optional 
	
	// Utility functions
	void    SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void    SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void    SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);

private:
	void checkCompileErrors(GLuint shader, std::string type);
	
};

#endif


