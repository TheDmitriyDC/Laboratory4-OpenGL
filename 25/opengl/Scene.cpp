#include "Scene.h"
#include <iostream>
#include <cmath>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW

#include "SceneLevel.h"




CScene::CScene(GLuint width, GLuint height)
{
	Width = width;
	Height = height;
	currentLevel = 1;

	NumberOfLevels = 1;
	
}

CScene::~CScene()
{
}

void CScene::Init()
{
	for (int i = 0; i < NumberOfLevels; i++) {
		CSceneLevel scene_level(Width, Height);
		scene_level.InitLevel(i);
		Levelvec.push_back(scene_level);

	}



}

void CScene::ProcessInput(GLfloat dt)
{
	
}

void CScene::Update(GLfloat dt)
{
}

void CScene::Render(Camera camera)
{
	int i_currentLevel= currentLevel - 1; 
	Levelvec[i_currentLevel].Render(camera);
	

}
