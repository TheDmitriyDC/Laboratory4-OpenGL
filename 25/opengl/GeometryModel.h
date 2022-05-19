#pragma once

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "SceneObject.h"


class GeometryModel : public CSceneObject
{
public:
	GeometryModel();
	~GeometryModel();
	
	void render(Camera camera);
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();

	


};

