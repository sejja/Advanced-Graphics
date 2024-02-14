//
//	SkyBox.frag
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright © 2021 . All Rights reserved
//

#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 oUVs;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;

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