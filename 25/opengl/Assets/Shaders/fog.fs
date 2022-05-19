#version 330 core

out vec4 color;
in vec4 mWorldPos;

void main()
{
   float d = distance(mWorldPos.xyz, cameraPos);
    float lerp = (d - 5.0f)/10.f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);}