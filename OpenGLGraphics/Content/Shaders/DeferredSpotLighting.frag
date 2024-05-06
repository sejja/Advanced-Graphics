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
layout(binding = 4) uniform sampler2D uShadowMap;

struct Light {
    vec3 mPosition;
    vec3 mDirection;
    vec3 mColor;
    vec3 mAttenuation;
    float mRadius;
    float mInnerAngle;
    float mOutterAngle;
    float mFallOff;
    bool mCastShadows;
};

layout (std140) uniform UniformBuffer {
	mat4 ubView;
	mat4 ubProjection;
    vec3 ubCameraPosition;
};


uniform Light uLight;
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
    const float bias = max(0.001 * (1.0 - dot(normal, uLight.mDirection)), 0.001);

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

    float shadow = 1;
    
    const vec3 lightDir = normalize(vec3(ubView * vec4(uLight.mPosition, 1)) - fragPos);
    const float att = pow(smoothstep(uLight.mRadius, 0, length(vec3(ubView * vec4(uLight.mPosition, 1)) - fragPos)), uLight.mFallOff);
    float spotlight =1;
    const float aplha = dot(-lightDir, normalize(vec3(ubView * vec4(uLight.mDirection, 1))));

                 if(uLight.mCastShadows)
                     shadow = 1 - ShadowCalculation(uShadowMatrix * (inverse(ubView) * vec4(fragPos, 1)), normal);

                //If the outer anngle is larger than the perpendicularity between the incident lightray and the viewers vector
			    if(aplha < cos(uLight.mOutterAngle))
				    spotlight = 0;
			    else if(aplha > cos(uLight.mInnerAngle))
				    spotlight = 1;
                //else, recalculate the incident gradient intensity
			    else
				    spotlight = pow((aplha-cos(uLight.mOutterAngle))/(cos(uLight.mInnerAngle)-cos(uLight.mOutterAngle)), uLight.mFallOff);

			    spotlight = clamp(spotlight,0,1);

    FragColor = texture(gAlbedoSpec, oUVs) * vec4( att 
            //ambient
            * ((spotlight * 
            //shadowmapping
            (shadow) 
            //difuse
            * (max(dot(normal, lightDir), 0.0) * uLight.mColor 
            //specular
            + uLight.mColor * pow(max(dot(normalize(vec3(ubView * vec4(ubCameraPosition, 1)) - fragPos), 
                reflect(-lightDir, normal)), 0.0), 32)))), 1.0);
} 