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
/*! Bloom Calculation
*
*   Extracts the bloom color aberration from the final color and the bloom channel
*/ //----------------------------------------------------------------------
vec4 bloom(vec4 finalcolor) {
    vec4 bloomColor = texture(bBloomTexture, oUVs).rgba;
    return mix(finalcolor, bloomColor, 0.75); // linear interpolation
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
    
    const vec3 lightDir = normalize(uLight.mPosition - fragPos);
    const float att = pow(smoothstep(uLight.mRadius, 0, length(uLight.mPosition - fragPos)), uLight.mFallOff);
    float spotlight =1;
    const float aplha = dot(-lightDir, normalize(uLight.mDirection));

                 if(uLight.mCastShadows)
                     shadow = 1 - ShadowCalculation(uShadowMatrix * vec4(fragPos, 1), normal);

                //If the outer anngle is larger than the perpendicularity between the incident lightray and the viewers vector
			    if(aplha < cos(uLight.mOutterAngle))
				    spotlight = 0;
			    else if(aplha > cos(uLight.mInnerAngle))
				    spotlight = 1;
                //else, recalculate the incident gradient intensity
			    else
				    spotlight = pow((aplha-cos(uLight.mOutterAngle))/(cos(uLight.mInnerAngle)-cos(uLight.mOutterAngle)), uLight.mFallOff);

			    spotlight = clamp(spotlight,0,1);

    FragColor = bloom(texture(gAlbedoSpec, oUVs) * vec4( att 
            //ambient
            * ((spotlight * 
            //shadowmapping
            (shadow) 
            //difuse
            * (max(dot(normal, lightDir), 0.0) * uLight.mColor 
            //specular
            + uLight.mColor * pow(max(dot(normalize(ubCameraPosition - fragPos), 
                reflect(-lightDir, normal)), 0.0), 32)))), 1.0));
} 