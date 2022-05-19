#include "TessModel.h"



TessModel::TessModel() {

	Position = glm::vec3(6.0f, -2.0f, 0.0f);
	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0, 1.0, 0.0f
	};
	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch
	
	/*
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindVertexArray(0);*/

	enablePosAttrib = true;
	enableNormAttrib = false;
	enableTextAttrib = false;
	CSceneObject::Init(points, 6);
}


TessModel::~TessModel()
{
}

void TessModel::render(Camera camera) {
	//glUseProgram(this->program);
	glm::mat4 model;
	model = glm::translate(model, Position);
	glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * model;

	ResourceManager::GetShader("TessShader").Use().SetMatrix4("mvp", mvp);
	//ResourceManager::GetShader("TessShader").Use().SetMatrix4("projectionMatrix", Projection);
	//ResourceManager::GetShader("TessShader").Use().SetMatrix4("modelViewMatrix", Modelview);
	//GLint mvLoc = glGetUniformLocation(program, "mvp");
	//glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}