#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 ViewPos;
out vec4 WorldPos;
out vec4 ObjectPos;
out vec3 planeNormal;

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

    planeNormal = mat3(transpose(inverse(view))) * mat3(transpose(inverse(model))) * aNormal; //black magic
}