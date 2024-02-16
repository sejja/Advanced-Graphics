//
//  DeferredLighting.vert
//  OpenGL Graphics
//
//  Created by Diego Revilla on 16/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 oUVs;
out vec4 oShadowCoord[8];

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Provides vertices for a full-screen quad. Easy?
*/ //----------------------------------------------------------------------
void main() {
    oUVs = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}