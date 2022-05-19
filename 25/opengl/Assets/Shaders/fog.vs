#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 mWorldPos;

void main()
{
   vec4 mWorldPos = model *vec4(position, 1.0); 
   gl_Position =  proj * view * worldPos;
} 