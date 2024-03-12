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
out float r;

float newPos (float labse){
    vec3 absolutePos = instancePosition + center;
    return (absolutePos.y + labse*sigma*(instancePosition.y + acceleration.y*labse));
}

float newRelation (float x){
    return ( ( -e*sqrt(x)*log(x) ) / 2 );
}

void main()
{
    //Set constants points
    absolutePos = instancePosition + center;
    const vec3 centerTop = vec3(center.x, center.y + height/2, center.z );

    float a = acceleration.y;
    float v0 = instancePosition.y;
    float limit = centerTop.y;
    float labse = delta * 1.0f;
    float k = (limit + absolutePos.y)/sigma;

    float deltaLimit = (-v0+sqrt(pow(v0,2)-4*a*k))/(2*a); //Bhaskara equation
    float ymax = newPos(deltaLimit);

    if(labse > deltaLimit){
        labse = mod(labse, deltaLimit);
    }

    float yf = newPos(labse);
    r = newRelation( (yf - (absolutePos.y) )/(ymax- (absolutePos.y) ) );
    finalParticlePos = vec3(absolutePos.x + (instancePosition.x*r) , yf, absolutePos.z + (instancePosition.z*r));

    gl_Position = projection * view * vec4(finalParticlePos, 1.0);

    float finalParticleSize =  (pointSize * r);
    gl_PointSize = finalParticleSize;
}
