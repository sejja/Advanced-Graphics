#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aSize;
layout (location = 2) in vec4 aColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 particleColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = aSize.x; // Asume que el tamaño de la partícula está en x
    particleColor = aColor;
}