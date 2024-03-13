#version 460 core
out vec4 FragColor;

//layout (location = 0) in vec3 instancePosition;

uniform vec4 particleColor; //This is the base color
uniform float height;

in vec3 finalParticlePos;
in vec3 absolutePos;
in vec3 instancePos;

in float r;

float normalizeRGB (float x){
    return (x/255);
}

float XZDiff (vec3 pos){
    return ( sqrt( pow(pos.x, 2) + pow(pos.z, 2) ));
}

void main()
{
    const vec4 maxParticleColor = vec4(normalizeRGB(255), normalizeRGB(57), normalizeRGB(0), 0.68);

    float maxDiff = XZDiff(vec3(absolutePos.x + instancePos.x, 0, absolutePos.z +  + instancePos.y));
    float currentDiff = XZDiff(finalParticlePos);
    float minDiff = XZDiff(absolutePos);

    vec4 colorDiff = maxParticleColor - particleColor;
    FragColor = particleColor + (colorDiff*r);
}

