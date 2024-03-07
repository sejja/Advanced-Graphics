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

    vec3 newPosition = absolutePos + (sigma * Vec);

    gl_Position = projection * view * vec4(newPosition, 1.0);
    gl_PointSize = pointSize;
}