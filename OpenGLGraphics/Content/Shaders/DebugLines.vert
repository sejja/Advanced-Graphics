#version 460 core

layout(location = 0) in vec3 attr_position;
layout(location = 0) uniform mat4 uniform_mvp;

void main() {
    vec4 vertex = vec4(attr_position, 1.0f);
    gl_Position = uniform_mvp * vertex;
}