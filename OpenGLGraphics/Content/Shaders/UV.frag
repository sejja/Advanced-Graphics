#version 450 core

out vec4 FragColor;
in vec2 oUVs;

void main() {
	FragColor = vec4(oUVs, 0.f, 1.f);
}