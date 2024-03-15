//
//	SkyBox.frag
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright © 2024 . All Rights reserved
//

#version 460 core

layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 oUVs;

layout (location = 0) uniform mat4 uTransform;
layout (location = 1) uniform mat4 uView;
layout (location = 2) uniform mat4 uModel;

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   The Text Coords are the model position (as a cube),
*        and the position is the orthogonal projection
*/ //----------------------------------------------------------------------
void main()
{
    oUVs = aPos;
    gl_Position = vec4(uTransform * uView * vec4(aPos, 1.0)).xyww;
}
