//
//  DebugDepth.vert
//  OpenGL Graphics
//
//  Created by Diego Revilla on 09/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

// ------------------------------------------------------------------------
/*! Post Render
*
*   EMPTY FUNCTION
*/ //----------------------------------------------------------------------
void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}