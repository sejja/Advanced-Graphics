#version 460 core
layout (location = 0) in vec3 instancePosition; // Relative position

uniform mat4 view;
uniform mat4 projection;
uniform vec3 center; //This is an absolute position of the center of the system
uniform vec3 acceleration; //This is an offset over acceleration
uniform float pointSize;
uniform float sigma;
//uniform float width;
uniform float height;
uniform int delta;

float newPos (float labse){
    vec3 absolutePos = instancePosition + center;
    return (absolutePos.y + labse*sigma*(instancePosition.y + acceleration.y*labse));
}

void main()
{
    //Set constants points
    const vec3 absolutePos = instancePosition + center;
    const vec3 centerTop = vec3(center.x, center.y + height/2, center.z );

    float a = acceleration.y;
    float v0 = instancePosition.y;
    float limit = centerTop.y;
    float labse = delta * 1.0f;
    float k = (limit + absolutePos.y)/sigma;

    float deltaLimit = (-v0+sqrt(pow(v0,2)-4*a*k))/(2*a); //Bhaskara equation

    if(labse > deltaLimit){
        labse = mod(labse, deltaLimit);
    }

    float yf = newPos(labse);

    vec3 newPosition = vec3(absolutePos.x, yf, absolutePos.z);

    gl_Position = projection * view * vec4(newPosition, 1.0);
    gl_PointSize = pointSize;
}
