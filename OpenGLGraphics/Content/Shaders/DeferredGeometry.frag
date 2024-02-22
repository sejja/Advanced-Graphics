//
//  DeferredGeometry.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 oUVs;
in vec3 oNormal;
in vec3 oPosition;
in vec3 oTangent;
in vec3 oBitangent;

uniform mat4 uModel;

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};

layout(binding = 0) uniform sampler2D uDiffuseTex;
layout(binding = 1) uniform sampler2D uNormalTex;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = oPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(inverse(transpose(mat3(ubView * uModel))) * 
        mat3(oTangent, oBitangent, oNormal) * 
        (texture(uNormalTex, oUVs).rgb * 2.0f - 1.0f));
    // and the diffuse per-fragment color
    gAlbedoSpec.rgba = texture(uDiffuseTex, oUVs).rgba;

    if(gAlbedoSpec.a < 0.1) discard;
}