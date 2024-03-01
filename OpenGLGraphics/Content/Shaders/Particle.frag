#version 460 core
out vec4 FragColor;

uniform vec4 particleColor; // Puedes ajustar el color aquí si es constante para todas las partículas o pasar como atributo

void main()
{
    FragColor = particleColor;
}