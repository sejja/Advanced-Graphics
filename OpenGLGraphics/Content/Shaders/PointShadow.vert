#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

out vec4 fragPos;

void main()
{
	gl_Position = model * vec4(aPos, 1.0);
	fragPos = gl_Position;
}