#ifndef __MODEL__
#define __MODEL__

#include "shader_s.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
using namespace std;

class Model
{
public:

    void Load(const char* filename);

    vector<Mesh> meshes;
   
private:

    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
};

#endif