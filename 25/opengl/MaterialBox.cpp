#include "MaterialBox.h"



MaterialBox::MaterialBox()
{
}


MaterialBox::~MaterialBox()
{
}

void MaterialBox::Init()
{
	shape = CUBE;
	enablePosAttrib = true;
	enableNormAttrib = true;
//	CSceneObject::init();
}

void MaterialBox::Render()
{
}
