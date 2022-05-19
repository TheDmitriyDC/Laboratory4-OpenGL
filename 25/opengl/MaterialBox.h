#pragma once
#pragma once
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

#include "SceneObject.h"
class MaterialBox :public CSceneObject
{
public:
	MaterialBox();
	~MaterialBox();
	void Init();
	void Render();
};

