//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"


const unsigned int width = 1600;
const unsigned int height = 900;

// Количество образцов на пиксель для MSAA
unsigned int samples = 8;

// Управляет гамма-функцией
float gamma = 2.2f;


float rectangleVertices[] =
{
	// Координаты // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

// Вершины для плоскости с текстурой
std::vector<Vertex> vertices =
{
	Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Индексы для плоскости с текстурой
std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Сообщите GLFW, какую версию OpenGL мы используем. 
	// В данном случае мы используем OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Используйте это только в том случае, если у вас нет фреймбуфера.
	//glfwWindowHint(GLFW_SAMPLES, samples);
	// Сообщите GLFW, что мы используем профиль CORE.
	// Это означает, что у нас есть только современные функции
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Создайте объект GLFWwindow размером 1600 на 900 пикселей, назвав его "Tutorial 26"
	GLFWwindow* window = glfwCreateWindow(width, height, "Tutorial 26", NULL, NULL);
	// Проверка на ошибку, если окно не удалось создать
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Ввести окно в текущий контекст
	glfwMakeContextCurrent(window);

	//Загрузите GLAD, чтобы он настроил OpenGL
	gladLoadGL();
	// Укажите область просмотра OpenGL в окне
	// В данном случае область просмотра изменяется от x = 0, y = 0, до x = 1600, y = 900
	glViewport(0, 0, width, height);





	// Генерирует шейдеры
	Shader shaderProgram("default.vert", "default.frag", "default.geom");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");

	// Позаботьтесь обо всем, что связано со светом
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	framebufferProgram.Activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);


	

	// Включает буфер глубины
	glEnable(GL_DEPTH_TEST);

	// Включает мультисэмплинг
	glEnable(GL_MULTISAMPLE);

	// Включает облицовку корпуса
	glEnable(GL_CULL_FACE);
	// Сохраняет лицевые поверхности
	glCullFace(GL_FRONT);
	// Используется стандарт счетчика часов
	glFrontFace(GL_CCW);


	// Создает объект камеры
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	

	// Подготовьте прямоугольник фреймбуфера VBO и VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



	// Переменные для создания периодического события для отображения FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Следит за количеством кадров во времениDiff
	unsigned int counter = 0;

	// Используйте это для отключения VSync (не рекомендуется)
	//glfwSwapInterval(0);


	// Создание объекта кадрового буфера
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Создание текстуры фреймбуфера
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Предотвращает кровотечение по краям
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Предотвращает кровотечение по краям
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Создание объекта буфера рендеринга
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Проверка фрейм-буфера на ошибки
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Создание объекта кадрового буфера
	unsigned int postProcessingFBO;
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Создание текстуры фреймбуфера
	unsigned int postProcessingTexture;
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// Проверка фрейм-буфера на ошибки
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;




	// Пути к текстурам
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string diffusePath = "/Resources/YoutubeOpenGL 27 - Normal Maps/textures/diffuse.png";
	std::string normalPath = "/Resources/YoutubeOpenGL 27 - Normal Maps/textures/normal.png";

	std::vector<Texture> textures =
	{
		Texture((parentDir + diffusePath).c_str(), "diffuse", 0)
	};

	// Плоскость с текстурой
	Mesh plane(vertices, indices, textures);
	// Карта нормалей для плоскости
	Texture normalMap((parentDir + normalPath).c_str(), "normal", 1);



	// Основной цикл while
	while (!glfwWindowShouldClose(window))
	{
		// Обновление счетчика и времени
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			// Создает новое название
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Tutorial 26";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Сброс времени и счетчика
			prevTime = crntTime;
			counter = 0;

			// Используйте это, если вы отключили VSync
			//camera.Inputs(window);
		}


		// Привязка пользовательского фреймбуфера
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// Укажите цвет фона
		glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
		// Очистите задний буфер и буфер глубины
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Включите тестирование глубины, поскольку оно отключено при рисовании прямоугольника фреймбуфера
		glEnable(GL_DEPTH_TEST);

		// Обработка входов камеры (удалите это, если вы отключили VSync)
		camera.Inputs(window);
		// Обновление и экспорт матрицы камеры в вершинный шейдер
		camera.updateMatrix(45.0f, 0.1f, 100.0f);



		shaderProgram.Activate();
		normalMap.Bind();
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "normal0"), 1);

		// Нарисуйте нормальную модель
		plane.Draw(shaderProgram, camera);

		// Сделайте так, чтобы FBO мультидискретизации считывался, пока рисуется FBO постобработки
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
		// Завершите мультисэмплинг и скопируйте его в FBO для постобработки
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// Привязка фреймбуфера по умолчанию
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Нарисуйте прямоугольник фреймбуфера
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // предотвращает отбрасывание прямоугольника фреймбуфера
		glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Поменяйте задний буфер на передний буфер
		glfwSwapBuffers(window);
		// Заботиться о проведении всех мероприятий GLFW
		glfwPollEvents();
	}



	// Удалите все объекты, которые мы создали
	shaderProgram.Delete();
	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &postProcessingFBO);
	// Удаление окна перед завершением программы
	glfwDestroyWindow(window);
	// Прекратите работу GLFW перед завершением программы
	glfwTerminate();
	return 0;
}