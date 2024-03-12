//
//	SkyBox.frag
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright © 2024 . All Rights reserved
//

#version 460 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 oUVs;

layout(binding = 0) uniform samplerCube uSkyBox;

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Samples the Skybox TextureCube (3D), as easy as that
*/ //----------------------------------------------------------------------
void main() {    
    FragColor = texture(uSkyBox, oUVs);
}