//
//  DebugDepth.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 20/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

out vec4 FragColor;
  
in vec2 oUVs;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;
layout(binding = 3) uniform sampler2D uShadowMap;

struct Light {
    vec3 mPosition;
    vec3 mDirection;
    vec3 mAmbient;
    vec3 mDiffuse;
    vec3 mSpecular;
    vec3 mAttenuation;
    float mInnerAngle;
    float mOutterAngle;
    float mFallOff;
    int mType;
};

uniform Light uDirectionalLight;
uniform vec3 uCameraPos;
uniform mat4 uShadowMatrix;

// ------------------------------------------------------------------------
/*! Shadow Calculation
*
*   Calculates the intensity of the scenes shadows for a given fragment
*/ //----------------------------------------------------------------------
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    const float bias = max(0.001 * (1.0 - dot(normal, uDirectionalLight.mDirection)), 0.001);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for(int x = -9; x <= 9; ++x)
      for(int y = -9; y <= 9; ++y)
          shadow += currentDepth - bias > texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r ? 1.0 : 0.0;        

    shadow /= pow(4, 4);

    //This should be fixed with Cascaded Shadow Maps?
    if(projCoords.z > 1.0) shadow = 0.0;

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
 
    vec3 totalLightShine = vec3(0, 0, 0);
    
    vec3 lightDir;
    float att = 1;
    float dist;
    float spotlight =1;

    lightDir = -uDirectionalLight.mDirection;
    dist = length(lightDir);

    totalLightShine += att * ((uDirectionalLight.mAmbient + spotlight * 
        (1 - ShadowCalculation(uShadowMatrix * vec4(fragPos, 1), normal)) 
        * max(dot(normal, lightDir), 0.0) * uDirectionalLight.mDiffuse + 
        uDirectionalLight.mSpecular * pow(max(dot(normalize(uCameraPos - fragPos), 
        reflect(-lightDir, normal)), 0.0), 32)));

    FragColor = texture(gAlbedoSpec, oUVs) * vec4(totalLightShine, 1.0);
} 