#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <GL/glew.h>
#include <glm/glm/glm.hpp>

#include "ResourceManager.h"
#include "camera.h"
#include "Texture.h"
enum ObjectShape {
	PYRAMID,
	CUBE,
	SPHERE,
};

enum Object_Movement {
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT
};
class CSceneObject
{
public:

	// Object state
	ObjectShape shape;

	glm::vec3   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	GLfloat MovementSpeed;
	// Render state
	CTexture   Sprite;
	// Constructor(s)
	CSceneObject();
	~CSceneObject();
	virtual void Init(GLfloat * vertices, int stride);
	//init cube
	virtual void Init();
	void ProcessKeyboard(Object_Movement direction, GLfloat deltaTime);
	// Draw sprite

	bool enablePosAttrib, enableNormAttrib, enableTextAttrib;
	GLuint m_VBO, m_VAO;
	GLuint m_Program;
};

#endif


