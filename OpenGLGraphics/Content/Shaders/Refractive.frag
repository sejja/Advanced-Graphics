//
//  DeferredGeometry.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/02/24
//  Copyright 2024. All rights reserved.
//

#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gBrightness;

layout (location = 5) in vec2 oUVs;
layout (location = 6) in vec3 oNormal;
layout (location = 7) in vec3 oPosition;
layout (location = 8) in vec3 oTangent;
layout (location = 9) in vec3 oBitangent;

layout(location = 0) uniform mat4 uModel;

layout (std140, binding = 0) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};

layout(binding = 0) uniform sampler2D uDiffuseTex;
layout(binding = 11) uniform samplerCube uCubemapReflection;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = oPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(inverse(transpose(mat3(ubView * uModel))) * 
        mat3(oTangent, oBitangent, oNormal) * 
        (oNormal));

    vec3 UVW = reflect(normalize(oPosition - ubCameraPosition), normalize(oNormal));
    UVW.y = 1- UVW.y;
    UVW.x = 1- UVW.x;
    // and the diffuse per-fragment color
    gAlbedoSpec.rgba = vec4(texture(uCubemapReflection, reflect(normalize(oPosition - ubCameraPosition), normalize(oNormal))).rgb, 1.0);
    gAlbedoSpec = pow(gAlbedoSpec, vec4(2.2));
    gBrightness = vec4(0.0);
}


