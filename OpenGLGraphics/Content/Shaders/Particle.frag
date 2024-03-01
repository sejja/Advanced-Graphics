#version 460 core
out vec4 FragColor;

in vec4 particleColor;

void main()
{
    FragColor = particleColor;
}