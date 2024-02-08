#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
    gl_Position = uTransform * uView * uModel * vec4(aPos, 1.0);
} 