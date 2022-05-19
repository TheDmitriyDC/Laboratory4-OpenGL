#version 330 core
out vec4 color;

// for Fog
in vec4 mWorldPos;
uniform vec3 viewPos;
void main()
{
    color = vec4(1.0f); // Set alle 4 vector values to 1.0f

    float d = distance(mWorldPos.xyz, viewPos);
    float lerp = (d - 5.0f)/5.0f;//10.f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);
}