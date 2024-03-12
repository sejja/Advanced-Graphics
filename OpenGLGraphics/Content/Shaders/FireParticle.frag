#version 460 core
out vec4 FragColor;

uniform vec3 center;
uniform vec4 particleColor; //This is the base color
uniform float height;

in vec3 finalParticlePos;
in float r;

float normalizeRGB (float x){
    return (x/255);
}

void main()
{
    const vec4 maxParticleColor = vec4(normalizeRGB(255), normalizeRGB(57), normalizeRGB(0), 0.68);
    vec3 centerTop = vec3(center.x, center.y + height/2, center.z);

    vec4 colorDiff = maxParticleColor - particleColor;
    FragColor = particleColor + colorDiff*r;
}