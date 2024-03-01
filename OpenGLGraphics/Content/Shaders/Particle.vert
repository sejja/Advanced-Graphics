#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Ajusta el tamaño de GL_Point aquí si es necesario, por ejemplo:
    // gl_PointSize = tamaño deseado;
}