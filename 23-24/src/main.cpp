#include <glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>
#include "CGame.h"
#include <iostream>
using namespace std;

CGame game;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    game.OnResize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        game.ReloadShaders();
    }
}

//int main(int argc, char* argv[])
int CALLBACK WinMain(
    __in  HINSTANCE hInstance,
    __in  HINSTANCE hPrevInstance,
    __in  LPSTR lpCmdLine,
    __in  int nCmdShow
)
{
    unsigned int screenWidth = 1280;
    unsigned int screenHeight = 920;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Tutorial 23-24", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    game.Initialize(screenWidth, screenHeight);

    while (!glfwWindowShouldClose(window))
    {
        game.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    game.Shutdown();

    glfwTerminate();

    return 0;
}