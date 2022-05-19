#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mvpMatrix;
uniform vec3 eyePos;
uniform vec3 lightDirection;
uniform mat4 lightViewProjMatrix;
uniform float time;

uniform vec3 diffuseColor;
uniform vec3 specularColor;

out vec3 fsNormal;
out vec3 fsUV;
out vec3 fsFragPos;
out vec4 fsShadowPos;

void main()
{
    vec4 pos = mvpMatrix * vec4(position.xyz, 1);
    fsNormal = vec3(modelMatrix * vec4(normal, 0)).xyz;
    fsFragPos = vec3(modelMatrix * vec4(position.xyz, 0)).xyz;

    gl_Position = pos;
}