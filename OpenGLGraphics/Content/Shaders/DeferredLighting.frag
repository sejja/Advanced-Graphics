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
uniform int uLightCount;
uniform vec3 uCameraPos;

void main() {     
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, oUVs).rgb;
    vec3 Normal = texture(gNormal, oUVs).rgb;
    vec3 Albedo = texture(gAlbedoSpec, oUVs).rgb;
    float Specular = texture(gAlbedoSpec, oUVs).a;
    
    vec3 totalLightShine = vec3(0, 0, 0);
    
    //Add per-light color using the Blinn-Phong equations
    for(int i = 0; i < uLightCount; i++) {
        float f = 0.0;
        //ambient
        float ambientStrength = 0.1;
        vec3 ambient = uLight[i].amb;
  
        //diffuse
        vec3 norm = Normal;
        
        vec3 lightDir;
        
       if(uLight[i].type == 2) {
            f = 0;
			lightDir = -uLight[i].dir;
		} else
		     lightDir = normalize(uLight[i].pos - FragPos); 

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * uLight[i].dif;
    
         //specular
        vec3 viewDir = normalize(uCameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = uLight[i].spe * spec;

        float dist = length(lightDir);
        float att = 1;
        
        if(uLight[i].type != 2) {
			 att = min(1.f / (uLight[i].att.x + uLight[i].att.y * dist + uLight[i].att.z * dist * dist), 1.0f);
		}
    
       float Spotlight =1;
		vec3 d = normalize(uLight[i].dir);
		if(uLight[i].type ==1)
		{
			float aplha = dot(-lightDir, d);

			if(aplha < cos(uLight[i].cosOut))
				Spotlight = 0;
			else if(aplha > cos(uLight[i].cosIn))
				Spotlight = 1;
			else
				Spotlight= pow((aplha-cos(uLight[i].cosOut))/(cos(uLight[i].cosIn)-cos(uLight[i].cosOut)), uLight[i].fallOff);

			Spotlight= clamp(Spotlight,0,1);
		}


        totalLightShine += att * ((ambient + Spotlight * (1 - f) * diffuse + specular));
   }

    FragColor = vec4(Albedo, Specular) * vec4(totalLightShine, 1.0);
} 