#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec4 fragPos;

void main()
{
	gl_Position =  uProj * uView * uModel * vec4(aPos, 1.0);
	fragPos = gl_Position;
}