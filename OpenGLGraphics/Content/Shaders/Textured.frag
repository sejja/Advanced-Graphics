#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUVs;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec2 aBitangent;

struct MaterialParameters {
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

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

uniform MaterialParameters uMaterial;
uniform Light uLight[8];
uniform vec3 uCameraPos;
uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;
uniform int uLightCount;

out vec4 FragColor;
in vec2 oUVs;
in vec3 oNormal;
in vec3 oPosition;
in vec3 oTangent;
in vec3 oBitangent;
in vec4 oShadowCoord[8];

layout(binding = 0) uniform sampler2D uDiffuseTex;
layout(binding = 1) uniform sampler2D uNormalTex;
layout(binding = 2) uniform sampler2D depth_texture2[8];

float ShadowCalculation(vec4 fragPosLightSpace, int light)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depth_texture2[0] , projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
  float bias = 0.001;

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(depth_texture2[light], 0);
  for(int x = -1; x <= 1; ++x)
  {
      for(int y = -1; y <= 1; ++y)
      {
          float pcfDepth = texture(depth_texture2[light], projCoords.xy + vec2(x, y) * texelSize).r; 
          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
      }    
  }
  shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
} 


void main() {
    mat4 VM = uView * uModel;
    mat3 VM3 = mat3(VM);
    mat3 iVM3 = inverse(transpose(VM3));
    mat3 V_M_TBN = iVM3 * mat3(oTangent, oBitangent, oNormal);
    vec3 N = normalize(V_M_TBN * (texture(uNormalTex, oUVs).rgb * 2.0f - 1.0f));

    vec3 totalLightShine = vec3(0, 0, 0);
    
    for(int i = 0; i < uLightCount; i++) {
        float f = ShadowCalculation(oShadowCoord[i], i);
        //ambient
        float ambientStrength = 0.1;
        vec3 ambient = uLight[i].amb;
  
        //diffuse
        vec3 norm = normalize(mat3(transpose(inverse(uModel))) * N);
        
        vec3 lightDir;
        
       if(uLight[i].type == 2) {
            f = 0;
			lightDir = -uLight[i].dir;
		} else
		     lightDir = normalize(uLight[i].pos - oPosition); 

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * uLight[i].dif;
    
         //specular
        vec3 viewDir = normalize(uCameraPos - oPosition);
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
  
    FragColor = texture(uDiffuseTex, oUVs) * vec4(totalLightShine, 1.0);
}