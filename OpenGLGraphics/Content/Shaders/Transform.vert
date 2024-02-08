#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUVs;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uShadowMatrix[8];
out vec2 oUVs;
out vec3 oNormal;
out vec3 oPosition;
out vec3 oTangent;
out vec3 oBitangent;
out vec4 oShadowCoord[8];

void main() {
	oUVs = aUVs;
	oNormal = aNormal;
	oPosition = vec3(uModel * vec4(aPos, 1));
	oTangent = aTangent;
	oBitangent = aBitangent;

	for(int i = 0; i < 8; i++)
		oShadowCoord[i] = uShadowMatrix[i] * vec4(oPosition, 1);
	gl_Position = uTransform * uView * uModel * vec4(aPos, 1.0);
}