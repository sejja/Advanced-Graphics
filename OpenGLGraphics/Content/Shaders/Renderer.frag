#version 460 core

out vec4 FragColor;

layout (location = 0) in vec2 texCoords;

layout (binding = 0) uniform sampler2D screenTexture;
layout(binding = 1) uniform sampler2D bBloomTexture;

uniform float exposure;

// ------------------------------------------------------------------------
/*! Bloom Calculation
*
*   Extracts the bloom color aberration from the final color and the bloom channel
*/ //----------------------------------------------------------------------
vec4 bloom(vec4 finalcolor) {
    vec4 bloomColor = texture(bBloomTexture, texCoords).rgba;
    return finalcolor + bloomColor; // linear interpolation
}

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, texCoords).rgb;
  
    // reinhard tone mapping

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    //gamma correction 

    mapped = pow(mapped, vec3(1.0 / gamma));
  
    //FragColor = vec4(1.0f, 0.f, 0.f, 1.f);

    FragColor = bloom(vec4(mapped , 1.0f));
}