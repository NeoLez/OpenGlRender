#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    float specularStrength = 0.5;
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor.xyz;

    float lightDistance = length(lightPos - WorldPos);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float attenuationFactor = (1 + 0.44 * lightDistance*lightDistance);
    float diffuse = clamp(clamp(dot(Normal, lightDir), 0, 1) / attenuationFactor, 0, 1);

    vec3 cameraDir = normalize(cameraPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;  

    vec3 result = (ambient + diffuse + specular) * lightColor.xyz * objectColor;

    FragColor = vec4( result, 1.0);
};