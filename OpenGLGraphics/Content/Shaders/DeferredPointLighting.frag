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
layout(binding = 3) uniform sampler2D gSSAO;
layout(binding = 4) uniform sampler2D uShadowMap;
layout(binding = 5) uniform samplerCube depthMap;

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

float ShadowCalculation(vec3 fragPos) {
    vec3 fragToLigth = vec3(inverse(ubView) * vec4(fragPos, 1)) - uLight.mPosition;
    float closestDepth = texture(depthMap, fragToLigth).r;
    closestDepth *= uLight.mRadius * 2;
    float currentDepth = length(fragToLigth);
    float bias = 0.07; 
    float shadow = currentDepth - bias < closestDepth ? 1.0 : 0.0; //Esto es un if raro.

    return shadow;
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
    const vec3 lightDir = normalize(vec3(ubView * vec4(uLight.mPosition, 1)) - fragPos);
    float AmbientOcclusion = texture(gSSAO, oUVs).r;

    float shadow = ShadowCalculation(fragPos);

    FragColor = texture(gAlbedoSpec, oUVs) * AmbientOcclusion * shadow * vec4(
            //atenuation
             pow(smoothstep(uLight.mRadius, 0, length(vec3(ubView * vec4(uLight.mPosition, 1)) - fragPos)), uLight.mFallOff) 
            //ambient
            * (( (max(dot(normal, lightDir), 0.0) * uLight.mColor
            //specular
            + uLight.mColor * pow(max(dot(normalize(vec3(ubView * vec4(ubCameraPosition, 1)) - fragPos), 

                reflect(-lightDir, normal)), 0.0), 32)))), 1.0);
//    vec3 fragToLigth = vec3(inverse(ubView) * vec4(fragPos, 1)) - uLight.mPosition;
//    float closestDepth = texture(depthMap, fragToLigth).r;
//    FragColor = vec4(vec3(closestDepth), 1.0);
} 