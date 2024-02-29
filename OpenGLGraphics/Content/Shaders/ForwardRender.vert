#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 0) in vec3 aNormal;
layout(location = 0) in vec2 aUVs;

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};
uniform mat4 uModel;

void main() {
	gl_Position = ubProjection * ubView * uModel * vec4(aPos, 1.0);
}