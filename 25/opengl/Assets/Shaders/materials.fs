#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct DirLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  

// for Fog
in vec4 mWorldPos;
//
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform PointLight light;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
  
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        

//    vec3 result = ambient + diffuse + specular;
//    color = vec4(result, 1.0f);

          // Attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant+ light.linear * dist + light.quadratic * (dist * dist));    

     ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;   
            
    color = vec4(ambient + diffuse + specular, 1.0f);  

 
     //fragment shader for fog
    float d = distance(mWorldPos.xyz,viewPos);// cameraPos);
    float lerp = (d - 5.0f)/5.0f;//10.0f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);
} 

