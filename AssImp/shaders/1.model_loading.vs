#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 2) in vec3 aTangent;
layout (location = 2) in vec3 aBitangent;

out vec2 TexCoords;
out vec4 ViewPos;
out vec4 WorldPos;
out vec4 ObjectPos;
out vec3 planeNormal;
out vec3 planeNormalw;
out vec3 planeTangent;
out vec3 planeTangentw;
out vec3 planeBitangent;
out vec3 planeBitangentw;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    
    ObjectPos = vec4(aPos, 1.0);
    WorldPos = model * ObjectPos;
    ViewPos = view * WorldPos;
    gl_Position = projection * ViewPos;


    planeNormalw = aNormal;
    planeTangentw = aTangent;
    planeBitangentw = aBitangent;
    mat3 normalToView =  mat3(transpose(inverse(view * model))); //black magic
    planeNormal = normalToView * aNormal;
    planeTangent = normalToView * aTangent;
    planeBitangent = normalToView * aBitangent;
}