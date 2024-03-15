//
//  White.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 15/02/24
//  Copyright 2024. All rights reserved.
//

#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

layout (location = 0) in vec2 oUVs;
layout (location = 1) in vec3 oNormal;
layout (location = 2) in vec3 oPosition;
layout (location = 3) in vec3 oTangent;
layout (location = 4) in vec3 oBitangent;

layout (location = 0) uniform vec3 uCameraPos;
layout (location = 1) uniform mat4 uTransform;
layout (location = 2) uniform mat4 uView;
layout (location = 3) uniform mat4 uModel;

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



























