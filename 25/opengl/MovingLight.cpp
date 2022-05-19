#include "MovingLight.h"



CMovingLight::CMovingLight()
{
}


CMovingLight::~CMovingLight()
{
}

void CMovingLight::Init()
{
	shape = CUBE;
	
	enablePosAttrib = true;
	enableNormAttrib = true;
//	CSceneObject::init();

}
