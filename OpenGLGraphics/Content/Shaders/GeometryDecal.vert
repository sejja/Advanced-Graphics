#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUVs;
layout (location = 3) in vec4 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 g_world;

layout (std140, binding = 0) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};

layout(location = 5) out vec3 WorldPos;
layout(location = 6) out vec2 TexCoords;
layout(location = 7) out vec4 ClipPos;

void main()
{
	gl_Position = ubProjection * ubView * g_world * vec4(aPosition, 1.0);
	WorldPos = (g_world * vec4(aPosition, 1.0)).xyz;
	vec3 N = normalize((g_world * vec4(aNormal, 0.0)).xyz);
	TexCoords = aUVs;
	vec3 T = normalize((g_world * vec4(aTangent.xyz, 0.0)).xyz);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T) * aTangent.w;
	ClipPos = gl_Position;
}
