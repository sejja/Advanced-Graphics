//
//  White.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 15/02/24
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
in vec4 oShadowCoord[8];

uniform vec3 uCameraPos;
uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;

layout(binding = 0) uniform sampler2D uDiffuseTex;
layout(binding = 1) uniform sampler2D uNormalTex;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = oPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = oNormal;
    // and the diffuse per-fragment color
    gAlbedoSpec.rgba = vec4(1, 1, 1, 1);
}