//
//	Transform.vert
//	OpenGL Graphics
//
//	Created by Diego Revilla on 19/02/24
//	Copyright © 2024 . All Rights reserved
//

#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUVs;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};

uniform mat4 uModel;

out vec2 oUVs;
out vec3 oNormal;
out vec3 oPosition;
out vec3 oTangent;
out vec3 oBitangent;

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Calculates the UVs, normal maps, the position and the tangent space	
*/ //----------------------------------------------------------------------
void main() {
    const vec4 pos = vec4(aPosition, 1.0);
	oUVs = aUVs;
	oNormal = aNormal;
	oPosition = vec3(uModel * pos);
	oTangent = aTangent;
	oBitangent = aBitangent;
	gl_Position = ubProjection * ubView * uModel * pos;
}