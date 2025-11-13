#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0); // 반드시 위치 먼저, 그다음 시점이동, 그다음 projection
}