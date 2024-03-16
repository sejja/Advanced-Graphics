/* Made by Lingfeng, but nobody knows how it works, good luck if you want to understand it
*/

#version 460 core
layout (location = 0) in vec3 instancePosition; // Relative position

uniform mat4 view;
uniform mat4 projection;
uniform vec3 center; //This is an absolute position of the center of the system
uniform vec3 acceleration;
uniform float pointSize;
uniform float sigma;
uniform float height;
uniform int delta;

const float e = 2.7182818284f;

out vec3 absolutePos;
out vec3 finalParticlePos;
out vec3 instancePos;
out float r;

float newPos (float labse){
    return (instancePosition.y + labse*sigma*(instancePosition.y + acceleration.y*labse));
}

float newRelation (float x){
    return ( ( e*sqrt(x)*log(x) ) / -2.0f );
}

void main()
{
    
    // Some output for the frag shader
    instancePos = instancePosition;
    absolutePos = instancePosition + center;

    // Calculate deltaLimit 
    float v0 = instancePosition.y;
    float a = acceleration.y;
    float k = (height - instancePosition.y)/sigma;
    float deltaLimit = ( -v0+sqrt( (v0*v0) + (4*a*k) ) ) / (2*a); //Bhaskara equation

    // Get the module
    float labse = delta * 1.0f;
    labse = mod(labse, deltaLimit);

    // New relative y-axis vector
    float yf = newPos(labse);

    // Some math aproximation
    r = newRelation( (yf - instancePosition.y)/height );

    // The final pos
    finalParticlePos = vec3(center.x + instancePosition.x*(1 + r), center.y + yf, center.z + instancePosition.z*(1 + r));
    gl_Position = projection * view * vec4(finalParticlePos, 1.0);

    // The final particle size
    float finalParticleSize =  (pointSize * r);
    gl_PointSize = finalParticleSize;

}