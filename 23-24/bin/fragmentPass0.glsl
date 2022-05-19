#version 330 core
out vec4 FragColor;

in vec3 fsNormal;
in vec3 fsFragPos;

uniform vec3 eyePos;
uniform vec3 lightDirection;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

vec3 calculateDirectionalLighting(vec3 lightDir, vec3 normal, vec3 viewDir)
{
	float di = clamp(dot(normal, lightDir), 0, 1);

	vec3 half = normalize( lightDir + viewDir );
	float si = clamp(dot(normal, half), 0, 1);

	float ki = 0.01;
	vec3 ambient = ki * vec3(1);
    vec3 diffuse = di * diffuseColor;
    vec3 specular = pow(si, 20.0) * vec3(1);

    vec3 finalColor = ambient + diffuse + specular;

    return finalColor;
}

void main()
{
	vec3 viewDir = normalize(eyePos - fsFragPos);
	vec3 normal = normalize(fsNormal);

	vec3 finalColor = calculateDirectionalLighting(-lightDirection, normal, viewDir);

    FragColor = vec4(finalColor, 1.0f);
} 