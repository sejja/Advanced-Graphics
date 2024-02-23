#version 460 core

out vec4 FragColor;

layout (location = 0) in vec2 texCoords;

layout (binding = 0) uniform sampler2D screenTexture;
uniform float gamma;
void main()
{
    vec3 hdrColor = texture(screenTexture, texCoords).rgb;
  
    // reinhard tone mapping

    vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

    //gamma correction 

    mapped = pow(mapped, vec3(1.0 / gamma));
  
    //FragColor = vec4(1.0f, 0.f, 0.f, 1.f);

    FragColor = vec4( (vec3(1.0f) - mapped) , 1.0f);
}