#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUVs;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 0) uniform mat4 uModel;
layout(location = 1) uniform mat4 uTransform;
layout(location = 2) uniform mat4 uView;
layout(location = 3) uniform mat4 uShadowMatrix[8];

layout(location = 5) out vec2 oUVs;
layout(location = 6) out vec3 oNormal;
layout(location = 7) out vec3 oPosition;
layout(location = 8) out vec3 oTangent;
layout(location = 9) out vec3 oBitangent;

void main() {
	oUVs = aUVs;
	oNormal = aNormal;
	oPosition = vec3(uModel * vec4(aPos, 1));
	oTangent = aTangent;
	oBitangent = aBitangent;
	gl_Position = uTransform * uView * uModel * vec4(aPos, 1.0);
}







