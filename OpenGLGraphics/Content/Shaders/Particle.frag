#version 460 core
out vec4 FragColor;

uniform vec4 particleColor; // Puedes ajustar el color aqu� si es constante para todas las part�culas o pasar como atributo

void main()
{
    FragColor = particleColor;
}