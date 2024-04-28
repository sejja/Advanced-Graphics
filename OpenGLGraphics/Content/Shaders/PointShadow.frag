#version 460 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
	float lightDistance = length(lightPos - fragPos.xyz);
	lightDistance = lightDistance / far_plane;
	gl_FragDepth = lightDistance;
}