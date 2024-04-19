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
layout(binding = 4) uniform sampler2DArray uShadowMap;
uniform int cascadeCount;
uniform float cascadePlaneDistances[16];
uniform mat4 lightSpaceMatrices[16];

struct Light {
    vec3 mDirection;
    vec3 mColor;
    bool mCastShadows;
};

uniform Light uLight;

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};

float ShadowCalculation(vec3 fragPosWorldSpace) {
    // select cascade layer
    vec4 fragPosViewSpace = ubView * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(texture(gNormal, oUVs).rgb);
    float bias = max(0.001 * (1.0 - dot(normal, uLight.mDirection)), 0.001);
    const float biasModifier = 0.5f;
    if (layer == cascadeCount)
    {
        bias *= 1 / (10000.f * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(uShadowMap, 0));
    for(int x = -9; x <= 9; ++x)
    {
        for(int y = -9; y <= 9; ++y)
        {
            float pcfDepth = texture(uShadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
     shadow /= pow(4, 4);
        
    return shadow;
}

// ------------------------------------------------------------------------
/*! Shader Entrypoint
*
*   Given already rendered geometry (gAlbedo), light the scene *Does magic*
*/ //----------------------------------------------------------------------
void main() {     
    // retrieve data from G-buffer
    const vec3 normal = texture(gNormal, oUVs).rgb;
    float shadow = 1.1 - ShadowCalculation(texture(gPosition, oUVs).rgb) * 0.75;
    float AmbientOcclusion = texture(gSSAO, oUVs).r;

    FragColor = texture(gAlbedoSpec, oUVs) * AmbientOcclusion * vec4(((shadow * (max(dot(normal, uLight.mDirection), 0.1) * uLight.mColor 
            //specular
            + uLight.mColor * pow(max(dot(normalize(ubCameraPosition - texture(gPosition, oUVs).rgb), 
                reflect(-uLight.mDirection, normal)), 0.0), 32)))), 1.0);
} 