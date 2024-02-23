#version 460 core

out vec4 FragColor;

layout (location = 0) in vec2 texCoords;

layout (binding = 0) uniform sampler2D screenTexture;

uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, texCoords).rgb;
  
    // reinhard tone mapping

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    //gamma correction 

    mapped = pow(mapped, vec3(1.0 / gamma));
  
    //FragColor = vec4(1.0f, 0.f, 0.f, 1.f);

    FragColor = vec4(mapped , 1.0f);
}