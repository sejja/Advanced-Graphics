//
//  Upsampling.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 1/03/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
	vec2 ubScreenSize;
};

uniform sampler2D srcTexture;
uniform float filterRadius;

in vec2 oUVs;
layout (location = 0) out vec3 upsample;

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Upsasmples an image, creating blur
*/ //----------------------------------------------------------------------
void main() {
    float x = filterRadius;
    float y = filterRadius;

    vec3 a = texture(srcTexture, vec2(oUVs.x - x, oUVs.y + y)).rgb;
    vec3 b = texture(srcTexture, vec2(oUVs.x,     oUVs.y + y)).rgb;
    vec3 c = texture(srcTexture, vec2(oUVs.x + x, oUVs.y + y)).rgb;

    vec3 d = texture(srcTexture, vec2(oUVs.x - x, oUVs.y)).rgb;
    vec3 e = texture(srcTexture, vec2(oUVs.x,     oUVs.y)).rgb;
    vec3 f = texture(srcTexture, vec2(oUVs.x + x, oUVs.y)).rgb;

    vec3 g = texture(srcTexture, vec2(oUVs.x - x, oUVs.y - y)).rgb;
    vec3 h = texture(srcTexture, vec2(oUVs.x,     oUVs.y - y)).rgb;
    vec3 i = texture(srcTexture, vec2(oUVs.x + x, oUVs.y - y)).rgb;

    upsample = e*4.0;
    upsample += (b+d+f+h)*2.0;
    upsample += (a+c+g+i);
    upsample *= 1.0 / 16.0;
}