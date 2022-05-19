#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 lightViewProjMatrix;

void main()
{
    gl_Position = lightViewProjMatrix * modelMatrix * vec4(position.xyz, 1);
}