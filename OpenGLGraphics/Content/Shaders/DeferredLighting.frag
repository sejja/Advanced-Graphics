//
//  DebugDepth.frag
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/02/24
//  Copyright � 2024. All rights reserved.
//

#version 460 core

out vec4 FragColor;
  
in vec2 TexCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 pos;
    vec3 dir;
    vec3 amb;
    vec3 dif;
    vec3 spe;
    vec3 att;
    float cosIn;
    float cosOut;
    float fallOff;
    int type;
};


uniform Light uLight[8];
uniform vec3 uCameraPos;

// ------------------------------------------------------------------------
/*! Shadow Calculation
*
*   Calculates the ammount of shadow depending on the fragment position and
*       the respective light index
*/ //----------------------------------------------------------------------
float ShadowCalculation(vec4 fragPosLightSpace, int lightidx) {
  // perform perspective divide, in [0,1] range
  const vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
    
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  
  // check whether current frag pos is in shadow
  const float bias = 0.001;

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(uShadowMaps[lightidx], 0);

  //For smooth corners, don't hard decide on the reference pixel (blend with neighbours)
  for(int x = -9; x <= 9; ++x) {
      for(int y = -9; y <= 9; ++y) {
          shadow += currentDepth - bias > texture(uShadowMaps[lightidx], projCoords.xy + vec2(x, y) * texelSize).r ? 1.0 : 0.0;        
      }    
  }
  shadow /= pow(3, 3);

  return projCoords.z <= 1.0 ? shadow : 0.0;
}

void main() {     
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    vec3 totalLightShine = vec3(0, 0, 0);
    
    //Add per-light color using the Blinn-Phong equations
    for(int i = 0; i < uLightCount; i++) {
        float shadowint = ShadowCalculation(oShadowCoord[i], i);
        //ambient
        vec3 ambient = uLight[i].amb;
  
        //diffuse
        vec3 norm = normalize(mat3(transpose(inverse(uModel))) * Nomal);
        
        vec3 lightDir;
        
        //Calculate the light's directional vector
        switch(uLight[i].type) {
        case 2:
            shadowint = 0;
		    lightDir = -uLight[i].dir;  
            break;
        default:
            lightDir = normalize(uLight[i].pos - oPosition); 
            break;
        }

        vec3 diffuse = max(dot(norm, lightDir), 0.0) * uLight[i].dif;
    
         //specular
        vec3 specular = uLight[i].spe * pow(max(dot(normalize(uCameraPos - oPosition),
            reflect(-lightDir, norm)), 0.0), 32);

        float dist = length(lightDir);
        float att = 1;
    
        float Spotlight =1;

        //Depending on the light type, treat the spotlight border (if we are within it)

        switch(uLight[i].type) {
            case 1:
            break;
            default:
				att = min(1.f / (uLight[i].att.x + uLight[i].att.y * dist + uLight[i].att.z * dist * dist), 1.0f);
				break;
        }

        switch(uLight[i].type) {
        case 1:
            float aplha = dot(-lightDir, normalize(uLight[i].dir));

			if(aplha < cos(uLight[i].cosOut))
				Spotlight = 0;
			else if(aplha > cos(uLight[i].cosIn))
				Spotlight = 1;
			else
				Spotlight= pow((aplha-cos(uLight[i].cosOut))/(cos(uLight[i].cosIn)-cos(uLight[i].cosOut)), uLight[i].fallOff);

			Spotlight= clamp(Spotlight,0,1);
            att = min(1.f / (uLight[i].att.x + uLight[i].att.y * dist + uLight[i].att.z * dist * dist), 1.0f);
            break;
        case 0:
             att = min(1.f / (uLight[i].att.x + uLight[i].att.y * dist + uLight[i].att.z * dist * dist), 1.0f);
             break;
        default:
    break;
        }

        totalLightShine += att * ((ambient + Spotlight * (1 - shadowint) * diffuse + specular));
   }

    FragColor = vec4(texture(uSkyBox, reflect(normalize(oPosition - uCameraPos), normalize(oNormal))).rgb, 1.0) * vec4(totalLightShine, 1.0);
} 