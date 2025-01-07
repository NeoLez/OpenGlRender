#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 ViewPos;
in vec4 WorldPos;

in vec3 planeNormal;
in vec3 planeTangent;
in vec3 planeBitangent;

in vec3 planeNormalw;
in vec3 planeTangentw;
in vec3 planeBitangentw;

struct PointLight {
    vec3 viewPosition;
    vec3 color;
    float attenuationCoefficient;
};

#define NR_LIGHTS 3
uniform PointLight lights[NR_LIGHTS];

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

float calculateLightInfluence(vec3 fragmentViewPosition, PointLight light) {
    vec3 diff = light.viewPosition - fragmentViewPosition;
    float distance = length(diff);

    return clamp(dot(normalize(diff), planeNormal), 0.0, 1.0)/(1 + light.attenuationCoefficient * distance * distance);
}

float calculateSpecularLightInfluence(vec3 fragmentViewPosition, PointLight light) {
    float specularStrength = texture(texture_specular1, TexCoords).x;
    vec3 viewDir = - ViewPos.xyz;
    vec3 lightDir = light.viewPosition - ViewPos.xyz;
    vec3 reflectDir = reflect(-normalize(lightDir), planeNormal);
    float specularStrengthAttenuation = (1 + (1/(light.attenuationCoefficient+1)) * length(viewDir) + length(lightDir));
    float spec = pow(max(dot(normalize(viewDir), reflectDir), 0.0), 8 * specularStrengthAttenuation);
    return specularStrength * spec * 1.5 / specularStrengthAttenuation;  
}

void main()
{   
    vec3 totalLightColor = vec3(0,0,0);
    for(int i = 0; i < NR_LIGHTS; i++) {
        totalLightColor += calculateLightInfluence(ViewPos.xyz, lights[i]) * lights[i].color;
    }
    vec3 totalSpecularLight = vec3(0,0,0);
    for(int i = 0; i < NR_LIGHTS; i++) {
        totalSpecularLight += calculateSpecularLightInfluence(ViewPos.xyz, lights[i]) * lights[i].color;
    }

    FragColor = vec4(totalLightColor + totalSpecularLight.xyz, 1.0f) * texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(planeTangentw, 1.0f);
    //float a = calculateLightInfluence(ViewPos.xyz, lights[0]);
    //FragColor = vec4((calculateLightInfluence(ViewPos.xyz, lights[0]) * lights[0].color) * texture(texture_diffuse1, TexCoords).xyz, 1.0f);
}