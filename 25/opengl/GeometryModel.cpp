#include "GeometryModel.h"




GeometryModel::GeometryModel() {

	GLfloat points[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //passing in 1 point
	};
	enablePosAttrib = true;
	enableNormAttrib = true;
	enableTextAttrib = false;
	CSceneObject::Init(points, 6);
}
GeometryModel::~GeometryModel()
{
}


void GeometryModel::render(Camera camera) {
	
	glm::mat4 model;
	
	model = glm::mat4();
	model = glm::translate(model, Position);
	glm::mat4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * model;
	ResourceManager::GetShader("StarShader").Use().SetMatrix4("mvp", mvp);
	ResourceManager::GetShader("StarShader").Use().SetMatrix4("model", model);
	ResourceManager::GetShader("StarShader").Use().SetVector3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void GeometryModel::setPosition(glm::vec3 pos)
{
	Position = pos;
}

glm::vec3 GeometryModel::getPosition()
{
	return glm::vec3(Position);
}
