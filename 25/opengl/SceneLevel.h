#ifndef SCENELEVEL_H
#define SCENELEVEL_H

#include <GL/glew.h>
#include <glm/glm/glm.hpp>

#include "ResourceManager.h"


#pragma once


class CSceneLevel
{
public:

	// Constructor& Destructor
	CSceneLevel(GLuint width, GLuint height);
	~CSceneLevel();
	void InitLevel(int level);
	
	// Loads level from file outside
	void      Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// Render level
	void      Render(Camera camera);
	// Check if the level is completed (all non-solid tiles are destroyed)
	GLboolean IsCompleted();


private:
	// Initialize level from tile data
	void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
	
	GLuint                 Width, Height;
	
	glm::vec3 lightPos;
	GLuint VBO, containerVAO;
	GLuint lightVAO;
	GLuint skyboxVAO, skyboxVBO;
	GLuint cubemapTexture;
};

#endif


