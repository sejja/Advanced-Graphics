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
layout (location = 3) out vec4 gBrightness;

in vec2 oUVs;
in vec3 oNormal;
in vec3 oPosition;
in vec3 oTangent;
in vec3 oBitangent;

uniform vec3 uCameraPos;
uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;
uniform vec4 uColor;

layout(binding = 0) uniform sampler2D uDiffuseTex;
layout(binding = 1) uniform sampler2D uNormalTex;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = oPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = oNormal;
    // and the diffuse per-fragment color
    gAlbedoSpec.rgba = vec4(1, 0, 0, 1);
    gBrightness.rgba = vec4(1, 0, 0, 1);
}