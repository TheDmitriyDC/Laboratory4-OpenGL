#include "camera.h"
#include <iostream>
#include <cmath>


//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW

#include <glm/glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
//Other lib
#include <SOIL/SOIL.h>

#include "SceneLevel.h"
#include "MaterialBox.h"
#include "LoadModel.h"
#include "GeometryModel.h"
#include "TessModel.h"



GeometryModel *geomModel;
TessModel *tessModel;
//MaterialBox matbox;


GLuint loadCubemap(vector<const GLchar*> faces);


CSceneLevel::CSceneLevel(GLuint width, GLuint height)
{
	Width = width, Height = height;
}


CSceneLevel::~CSceneLevel()
{
}

void CSceneLevel::InitLevel(int level)
{	// Атрибуты света
	lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	//// Создать и скомпилировать нашу шейдерную программу
	ResourceManager::LoadShader("Assets/Shaders/materials.vs", "Assets/Shaders/materials.fs", nullptr, "lightingShader");
	ResourceManager::LoadShader("Assets/Shaders/ptlight.vs", "Assets/Shaders/ptlight.fs", nullptr, "ptLightShader");
	ResourceManager::LoadShader("Assets/Shaders/lamp.vs", "Assets/Shaders/lamp.fs", nullptr, "lampShader");
	ResourceManager::LoadShader("Assets/Shaders/model.vs", "Assets/Shaders/model.fs", nullptr, "modelShader");
	ResourceManager::LoadShader("Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.fs", nullptr, "SkyboxShader");
	ResourceManager::LoadShader("Assets/Shaders/tess_vert.vs", "Assets/Shaders/tess_frag.fs", nullptr, "Assets/Shaders/tess_quad.tcs", "Assets/Shaders/tess_quad.tes", "TessShader");
	//ResourceManager::LoadShader("Assets/Shaders/star.vs", "Assets/Shaders/star.fs", "Assets/Shaders/star.gs", "StarShader");
	// геометрическая модель звезды
	geomModel = new GeometryModel();
	geomModel->setPosition(glm::vec3(6.0f, 1.0f, 0.0f));

	//тесс 
	tessModel = new TessModel();

	GLfloat vertices[] = {
		// Позиции           // Нормы           // Координаты текстуры
		// Задняя поверхность
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		// Передняя часть
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		// Левая сторона
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		// Правая сторона
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		// Нижняя сторона
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		// Верхняя часть
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
	};

	// Вершины скайбокса
	GLfloat skyboxVertices[] = {
		// Позиции          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	// Сначала установить VAO (и VBO) контейнера.

	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Атрибут позиции
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Нормальный атрибут
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//VAO света (VBO остается неизменным. В конце концов, вершины одинаковы для объекта света (также 3D куба)).

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// Нам нужно только привязать VBO (чтобы связать его с glVertexAttribPointer), заполнять его не нужно; данные VBO уже содержат все, что нам нужно.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Установить атрибуты вершин (только данные о положении для лампы))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // мы пропускаем нормальные векторы
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);



	// Настройка скайбокса VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap/Skybox
	vector<const GLchar*> faces;
	faces.push_back("Assets/Skybox/right.jpg");
	faces.push_back("Assets/Skybox/left.jpg");
	faces.push_back("Assets/Skybox/top.jpg");
	faces.push_back("Assets/Skybox/bottom.jpg");
	faces.push_back("Assets/Skybox/back.jpg");
	faces.push_back("Assets/Skybox/front.jpg");
	 cubemapTexture = loadCubemap(faces);

}

void CSceneLevel::Load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
}

void CSceneLevel::Render(Camera camera)
{
	
	// Сначала нарисовать скайбокс
	glDepthMask(GL_FALSE);// отключить написание глубины для скайбокса
	
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	//Удалить любой компонент перевода матрицы представления
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 300.0f);

						  // Активировать шейдер скайбокса
	
	ResourceManager::GetShader("SkyboxShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("SkyboxShader").Use().SetMatrix4("projection", projection);

	// Куб Skybox
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetShader("SkyboxShader").Use().SetInteger("skybox", 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);

	
	 // Изменение значений позиции света во времени (может быть сделано в любом месте игрового цикла, но сделать это по крайней мере до использования позиций источников света)
	
	
	Camera_Movement dir = camera.Getlightmovement();
	if (dir == FORWARD)
	{
		lightPos.y = lightPos.y + 0.01f;
		dir = NONE;
	}
	if (dir == BACKWARD)
	{
		lightPos.y = lightPos.y - 0.01f;
		dir = NONE;
	}
	if (dir == LEFT)
	{
		lightPos.x = lightPos.x - 0.01f;
		dir = NONE;
	}
	if (dir == RIGHT)
	{
		lightPos.x = lightPos.x + 0.01f;
		dir = NONE;
	}


	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.position", lightPos.x, lightPos.y, lightPos.z);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);


	// Установить свойства освещения
	glm::vec3 lightColor;
	lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Уменьшить влияние
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Низкое влияние
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);
	ResourceManager::GetShader("lightingShader").Use().SetFloat("light.constant", 1.0f);
	ResourceManager::GetShader("lightingShader").Use().SetFloat("light.linear", 0.09f);
	ResourceManager::GetShader("lightingShader").Use().SetFloat("light.quadratic", 0.032f);

	// Установить свойства материала
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("material.specular", 0.5f, 0.5f, 0.5f); // Specular не имеет полного эффекта на материале этого объекта
	ResourceManager::GetShader("lightingShader").Use().SetFloat("material.shininess", 32.0f);

	// Создание трансформаций камеры

	view = camera.GetViewMatrix();
	//projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	// Получить места расположения униформы

	ResourceManager::GetShader("lightingShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("lightingShader").Use().SetMatrix4("projection", projection);
	glBindVertexArray(containerVAO);
	glm::mat4 model;

	ResourceManager::GetShader("lightingShader").Use().SetMatrix4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);




	// Также нарисовать объект лампы, снова привязав соответствующий шейдер

	ResourceManager::GetShader("lampShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("lampShader").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("lampShader").Use().SetVector3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

	model = glm::mat4();
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // Сделать кубик поменьше

	ResourceManager::GetShader("lampShader").Use().SetMatrix4("model", model);
	// Нарисовать объект света (используя вершинные атрибуты света)
	glBindVertexArray(lightVAO);


	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0.5, 0);*/
	geomModel->render(camera);

	tessModel->render(camera);

}

GLboolean CSceneLevel::IsCompleted()
{
	return GLboolean();
}

GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void CSceneLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
}
