#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
// GLM Математика
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class CSceneLevel;

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};
class CScene
{
public:
	// Состояние игры
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	// вектор
	std::vector<CSceneLevel> Levelvec;
	
	// Конструктор/деструктор
	CScene(GLuint width, GLuint height);
	~CScene();
	// Инициализировать состояние игры (загрузить все шейдеры/текстуры/уровни)
	void Init();
	// Игровая петля
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render(Camera camera);

private:
	int					NumberOfLevels;
	int					currentLevel;

	
};

#endif


