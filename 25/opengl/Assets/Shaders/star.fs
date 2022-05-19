#version 330 core

in vec3 outColor;
out vec4 color;

in vec4 mWorldPos;
uniform vec3 viewPos;


void main(){
    //color = vec4(outColor, 1.0f);
    color = vec4 (1.0f, 1.0f, 0.0f, 1.0f);

      //fragment shader for fog
    float d = distance(mWorldPos.xyz,viewPos);// cameraPos);
    float lerp = (d - 5.0f)/5.0f;//10.0f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);
}