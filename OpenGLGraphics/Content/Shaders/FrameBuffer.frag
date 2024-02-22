#version 460 core

out vec4 FragColor;

layout (location = 1) in vec2 texCoords;

uniform float gamma;
uniform sampler2D screenTexture;

void main()
{
    vec3 hdrColor = texture(screenTexture, texCoords).rgb;
  
    // reinhard tone mapping

    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    // gamma correction 

    mapped = pow(mapped, vec3(2.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}