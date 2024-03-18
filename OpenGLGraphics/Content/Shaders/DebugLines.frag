#version 460 core

out vec4 out_color;

layout(location = 1) uniform vec4 uniform_color;

void main() {
    out_color = uniform_color;
}