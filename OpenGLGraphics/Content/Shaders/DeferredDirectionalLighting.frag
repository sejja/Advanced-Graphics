//
//  DebugDepth.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

out vec4 FragColor;
  
in vec2 oUVs;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;
layout(binding = 3) uniform sampler2D bBloomTexture;
layout(binding = 4) uniform sampler2D uShadowMap;

struct Light {
    vec3 mDirection;
    vec3 mColor;
    bool mCastShadows;
};

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};


uniform Light uLight;

// ------------------------------------------------------------------------
/*! Bloom Calculation
*
*   Extracts the bloom color aberration from the final color and the bloom channel
*/ //----------------------------------------------------------------------
vec4 bloom(vec4 finalcolor) {
    vec4 bloomColor = texture(bBloomTexture, oUVs).rgba;
    return mix(finalcolor, bloomColor, 0.05); // linear interpolation
}

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Given already rendered geometry (gAlbedo), light the scene *Does magic*
*/ //----------------------------------------------------------------------
void main() {     
    // retrieve data from G-buffer
    const vec3 normal = texture(gNormal, oUVs).rgb;

    FragColor = bloom(texture(gAlbedoSpec, oUVs) * vec4((((max(dot(normal, -uLight.mDirection), 0.0) * uLight.mColor 
            //specular
            + uLight.mColor * pow(max(dot(normalize(ubCameraPosition - texture(gPosition, oUVs).rgb), 
                reflect(uLight.mDirection, normal)), 0.0), 32)))), 1.0));
} 