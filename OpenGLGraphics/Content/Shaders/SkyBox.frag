//
//	SkyBox.frag
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright © 2024 . All Rights reserved
//

#version 460 core

out vec4 FragColor;

in vec3 oUVs;

uniform samplerCube skybox;

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Samples the Skybox TextureCube (3D), as easy as that
*/ //----------------------------------------------------------------------
void main() {    
    FragColor = texture(skybox, oUVs);
}