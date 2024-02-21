#version 460 core

out vec4 FragColor;
layout (location = 1) in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float gamma;

void main()
{
    vec4 fragment = texture(screenTexture, texCoords);
    FragColor.rgb = pow(fragment.rgb, vec3(1.0f / 2.2f));
    FragColor = vec4(gamma) - texture(screenTexture, texCoords);
}