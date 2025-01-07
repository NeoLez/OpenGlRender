#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 ViewPos;
in vec4 WorldPos;

in vec3 planeNormal;

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

    return dot(normalize(diff), planeNormal)/(1 + light.attenuationCoefficient * distance * distance);
}

void main()
{   
    vec3 totalLightColor = vec3(0,0,0);
    for(int i = 0; i < NR_LIGHTS; i++) {
        totalLightColor += calculateLightInfluence(ViewPos.xyz, lights[i]) * lights[i].color;
    }

    FragColor = vec4(totalLightColor.xyz, 1.0f) * texture(texture_diffuse1, TexCoords);

    //float a = calculateLightInfluence(ViewPos.xyz, lights[0]);
    //FragColor = vec4((calculateLightInfluence(ViewPos.xyz, lights[0]) * lights[0].color) * texture(texture_diffuse1, TexCoords).xyz, 1.0f);
}