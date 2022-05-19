#ifndef __MESH__
#define __MESH__

#include "shader_s.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    /*  Данные сетки  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    /*  Функции  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
    void Draw(Shader& shader);

private:
    /*  Рендеринг данных  */
    unsigned int VAO, VBO, EBO;
    /*  Функции    */
    void SetupMesh();
};

#endif