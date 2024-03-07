#version 460 core
layout (location = 0) in vec3 instancePosition; // Relative position

uniform mat4 view;
uniform mat4 projection;
uniform vec3 center;
uniform vec3 acceleration;
uniform float pointSize;
uniform float sigma;
uniform int delta;



vec3 instanceVelocity = vec3(instancePosition.x, abs(instancePosition.y), instancePosition.z);

void main()
{
    vec3 absolutePos = instancePosition + center;
    
    vec3 Vec = instanceVelocity +  (delta * acceleration);

    vec3 newPosition = absolutePos + (sigma * Vec); //falta capar delta, en este caso al ser la velocidad inicial muy bajo el valor de sigma practicamente lo anula

    gl_Position = projection * view * vec4(newPosition, 1.0);
    gl_PointSize = pointSize;
}