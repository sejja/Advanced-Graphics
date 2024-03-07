#version 460 core
layout (location = 0) in vec3 instancePosition; // Particle position
layout (location = 1) in vec3 instanceVelocity; // Particle velocity

uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;
uniform vec3 center;

void main()
{
    vec3 newPosition = instancePosition + center + instanceVelocity;
    gl_Position = projection * view * vec4(newPosition, 1.0);
    gl_PointSize = pointSize;
}