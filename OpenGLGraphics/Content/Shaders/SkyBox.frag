//
//	SkyBox.frag
//	OpenGL Graphics
//
//	Created by Diego Revilla on 12/02/24
//	Copyright © 2021 . All Rights reserved
//

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {    
    FragColor = texture(skybox, TexCoords);
}