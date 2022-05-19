#pragma once

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "SceneObject.h"

class TessModel: public CSceneObject
{

public:
	TessModel();
	~TessModel();
	void render(Camera camera);
};

