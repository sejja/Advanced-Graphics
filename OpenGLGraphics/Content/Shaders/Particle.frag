#version 460 core
out vec4 FragColor;

uniform vec4 particleColor;

void main()
{
    FragColor = particleColor;
}