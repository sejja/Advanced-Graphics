//
//  DebugDepth.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 07/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

out vec4 o_fragcolor;

in vec2 in_uv;

uniform sampler2D s_depthMap;

void main() {             
    o_fragcolor = vec4(vec3( texture(s_depthMap, in_uv).r), 1.0);
}