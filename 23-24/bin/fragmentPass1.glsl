#version 330 core
out vec4 FragColor;

in vec3 fsNormal;
in vec3 fsUV;
in vec3 fsFragPos;
in vec4 fsShadowPos;

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

uniform sampler2D shadowMapTexture;

float generateCheckerboard(vec2 uv)
{
	vec2 p = floor(uv*100.0);
	float s = mod( p.x + p.y, 2.0 );
	return s;
}

vec3 calculateDirectionalLighting(vec3 lightDir, vec3 normal, vec3 viewDir, vec2 uv, float shadowIntensity)
{
	normal = vec3(0,1,0);
	float di = clamp(dot(normal, lightDir), 0, 1);

	vec3 half = normalize( lightDir + viewDir );
	float si = clamp(dot(normal, half), 0, 1);

	float ki = 0.2;
	vec3 ambient = (ki * vec3(1.0)) * (1.0-shadowIntensity);
    vec3 diffuse = di * (diffuseColor * generateCheckerboard(uv));
    vec3 specular = pow(si, 10.0) * vec3(0.5);

    vec3 finalColor = ambient + diffuse + specular;

    return finalColor;
}

float calculateShadowIntensity(vec4 shadowPos)
{
	vec3 shadowCoord = (shadowPos.xyz/shadowPos.w) * 0.5 + 0.5;
	float currentDepth = shadowCoord.z;
	float closestDepth = texture(shadowMapTexture, shadowCoord.xy).r;

	if(currentDepth > 1.0)
	{
		return 0.0;
	}

	float bias = 0.0001;
	float shadow = 0.0;
	if((currentDepth - bias) > closestDepth)
	{
		shadow = 1.0;
	}

	return shadow;
}

void main()
{
	vec3 viewDir = normalize(eyePos - fsFragPos);
	vec3 normal = normalize(fsNormal);

	float shadowIntensity = calculateShadowIntensity(fsShadowPos);
	vec3 finalColor = calculateDirectionalLighting(-lightDirection, normal, viewDir, fsUV.xy, shadowIntensity);

    FragColor = vec4(finalColor, 1.0f);
} 