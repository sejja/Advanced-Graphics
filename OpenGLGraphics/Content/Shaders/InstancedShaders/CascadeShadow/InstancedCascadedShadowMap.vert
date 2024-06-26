//
//	Shadow.vert
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright � 2024 . All Rights reserved
//

#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 uModel;

layout(std140, binding = 1) uniform TransformBlock {
    mat4 instancedGeometryPos[100]; 
};

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Only transorms the vertex position from model position to viewport position
*/ //----------------------------------------------------------------------
void main() {
    gl_Position = instancedGeometryPos[gl_InstanceID] * vec4(aPos, 1.0);
} 