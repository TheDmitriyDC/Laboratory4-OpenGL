

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "camera.h"
#include "ResourceManager.h"
#include "Scene.h"



// Прототипы функций
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void do_light_movement();
void back_Culling();
bool enableCulling = false;
GLuint generateMultiSampleTexture(GLuint samples);

// Размеры окна
const GLuint WIDTH = 1920, HEIGHT = 1080;

// Камера
Camera  camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];


// Deltatime
GLfloat deltaTime = 0.0f;	// Время между текущим и последним кадром
GLfloat lastFrame = 0.0f;  	// Время последнего кадра


//АТИАЛИЗИРУЮЩИЙ кадровый буфер
GLuint framebuffer;


CScene game(WIDTH, HEIGHT);
							// Функция MAIN, отсюда мы запускаем приложение и запускаем игровой цикл
int main()
{
	camera.lightmovement = NONE;
	// Инициализация GLFW
	glfwInit();
	// Установите все необходимые параметры для GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Создайте объект GLFWwindow, который мы можем использовать для функций GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Установите необходимые функции обратного вызова
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Опции GLFW
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Установить значение true, чтобы GLEW знал, что нужно использовать современный подход к получению указателей функций и расширений
	glewExperimental = GL_TRUE;
	// Инициализация GLEW для установки указателей функций OpenGL
	glewInit();
	glGetError(); // Вызвав его один раз, чтобы поймать ошибку glewInit(), все остальные ошибки теперь исходят от нашего приложения.
	// Определить размеры области просмотра
	glViewport(0, 0, WIDTH, HEIGHT);

	
	// Параметры OpenGL
	glEnable(GL_MULTISAMPLE); // мультисэмплинг для антиалиасинга
	glEnable(GL_DEPTH_TEST);

	
	game.Init();

	game.State = GAME_ACTIVE;



	// Антиалиасинг
	// Фрейм-буферы

	// Фрейм-буферы

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Создание мультисэмплированной текстуры вложения цвета
	GLuint textureColorBufferMultiSampled = generateMultiSampleTexture(4);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
	// Создание объекта renderbuffer для вложений глубины и трафарета
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	while (!glfwWindowShouldClose(window))
	{
		// Вычислить время задержки текущего кадра
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Проверить, были ли активированы какие-либо события (нажатие клавиши, перемещение мыши и т.д.), и вызовите соответствующие функции реагирования
		glfwPollEvents();
		do_movement();
		do_light_movement();
		back_Culling();

		game.ProcessInput(deltaTime);
		game.Update(deltaTime);
		// Очистить буфер цветов

		// 1. Рисуйте сцену как обычно в буферах с мультисэмплированием
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.Render(camera);

		// 2. Теперь мультисэмплированный буфер(ы) передается в фреймбуферы по умолчанию
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	
		glfwSwapBuffers(window);
	}
	
	

	// Удалите все ресурсы, загруженные с помощью менеджера ресурсов
	ResourceManager::Clear();
	// Завершите работу GLFW, очистив все ресурсы, выделенные GLFW.
	glfwTerminate();
	return 0;
}

// Вызывается при каждом нажатии/отпускании клавиши через GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void back_Culling()
{
	if (keys[GLFW_KEY_B])
	{
		if (enableCulling == false)
		{
			enableCulling = true;
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}
		else
		{
			enableCulling = false;
			glDisable(GL_CULL_FACE);
		}
		
	}
		

}
void do_movement()
{
	// Элементы управления камерой
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	

}
void do_light_movement()
{
	if (keys[GLFW_KEY_I])
		camera.Setlightmovement(FORWARD);
	if (keys[GLFW_KEY_K])
		camera.Setlightmovement(BACKWARD);
	if (keys[GLFW_KEY_J])
		camera.Setlightmovement(LEFT);
	if (keys[GLFW_KEY_L])
		camera.Setlightmovement(RIGHT);
}


GLuint generateMultiSampleTexture(GLuint samples)
{
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, WIDTH, HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	return texture;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos;  // Перевернутая, так как координаты y идут снизу вверх слева направо

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((GLfloat)yoffset);
}