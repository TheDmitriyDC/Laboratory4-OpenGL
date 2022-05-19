#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, TexCoords);

   //fragment shader for fog

    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, 0.7);
}
  