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
layout(binding = 4) uniform sampler2D uShadowMaps[8];

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

uniform Light uLight[8];
uniform int uLightCount;
uniform vec3 uCameraPos;
uniform mat4 uShadowMatrix[8];

// ------------------------------------------------------------------------
/*! Shadow Calculation
*
*   Calculates the intensity of the scenes shadows for a given fragment
*/ //----------------------------------------------------------------------
float ShadowCalculation(vec4 fragPosLightSpace, int light, vec3 normal) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    const float bias = max(0.001 * (1.0 - dot(normal, uLight[light].mDirection)), 0.001);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMaps[light], 0);
    for(int x = -9; x <= 9; ++x)
      for(int y = -9; y <= 9; ++y)
          shadow += currentDepth - bias > texture(uShadowMaps[light], projCoords.xy + vec2(x, y) * texelSize).r ? 1.0 : 0.0;        

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
 
    vec3 totalLightShine = vec3(0, 0, 0);
    
    //Add per-light color using the Blinn-Phong equations
    for(int i = 0; i < uLightCount; i++) { 
        vec3 lightDir;
        float att = 1;
        float dist;
        float spotlight =1;

        //If this is a directional light type, we know the variance is nill (infinite)
        switch(uLight[i].mType) {
            case 2:
                lightDir = -uLight[i].mDirection;
                dist = length(lightDir);
                break;
            case 0:
                lightDir = normalize(uLight[i].mPosition - fragPos); 
                dist = length(lightDir);
                att = min(1.f / (uLight[i].mAttenuation.x + uLight[i].mAttenuation.y * dist + uLight[i].mAttenuation.z * dist * dist), 1.0f);
                break;
            default:
                lightDir = normalize(uLight[i].mPosition - fragPos); 
                dist = length(lightDir);
                att = min(1.f / (uLight[i].mAttenuation.x + uLight[i].mAttenuation.y * dist + uLight[i].mAttenuation.z * dist * dist), 1.0f);
       
                const float aplha = dot(-lightDir, normalize(uLight[i].mDirection));

                //If the outer anngle is larger than the perpendicularity between the incident lightray and the viewers vector
			    if(aplha < cos(uLight[i].mOutterAngle))
				    spotlight = 0;
			    else if(aplha > cos(uLight[i].mInnerAngle))
				    spotlight = 1;
                //else, recalculate the incident gradient intensity
			    else
				    spotlight = pow((aplha-cos(uLight[i].mOutterAngle))/(cos(uLight[i].mInnerAngle)-cos(uLight[i].mOutterAngle)), uLight[i].mFallOff);

			    spotlight = clamp(spotlight,0,1);
           }


        totalLightShine += att * ((uLight[i].mAmbient + spotlight * 
            (1 - ShadowCalculation(uShadowMatrix[i] * vec4(fragPos, 1), i, normal)) 
            * max(dot(normal, lightDir), 0.0) * uLight[i].mDiffuse + 
            uLight[i].mSpecular * pow(max(dot(normalize(uCameraPos - fragPos), 
            reflect(-lightDir, normal)), 0.0), 32)));
   }
    FragColor = bloom(texture(gAlbedoSpec, oUVs) * vec4(totalLightShine, 1.0));
} 