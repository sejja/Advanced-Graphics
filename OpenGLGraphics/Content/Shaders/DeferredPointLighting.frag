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
    vec3 mPosition;
    vec3 mColor;
    vec3 mAttenuation;
    float mRadius;
    float mFallOff;
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
    const vec3 fragPos = texture(gPosition, oUVs).rgb;
    const vec3 normal = texture(gNormal, oUVs).rgb;
    const vec3 lightDir = normalize(uLight.mPosition - fragPos);

    FragColor = bloom(texture(gAlbedoSpec, oUVs) * vec4(//atenuation
             pow(smoothstep(uLight.mRadius, 0, length(uLight.mPosition - fragPos)), uLight.mFallOff) 
            //ambient
            * (( (max(dot(normal, lightDir), 0.0) * uLight.mColor 
            //specular
            + uLight.mColor * pow(max(dot(normalize(ubCameraPosition - fragPos), 
                reflect(-lightDir, normal)), 0.0), 32)))), 1.0));
} 