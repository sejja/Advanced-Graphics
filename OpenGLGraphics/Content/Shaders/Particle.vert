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
    vec3 absolutePos = instancePosition + center;
    vec3 centerTop = vec3(center.x, center.y + height/2, center.z );

    /*
    vec3 instanceAcceleration = ((centerTop - absolutePos) + acceleration);
    vec3 instanceVelocity = vec3(instancePosition.x, abs(instancePosition.y), instancePosition.z);
    
    float labse = delta * 1.0f;
    //if(delta > 20){
        //labse = mod(delta, 20);
    //}
    vec3 VEC = ((labse/2)*instanceVelocity)  +  (labse * instanceAcceleration);
    VEC = sigma * VEC; //Sigma filter
    */

    float a = acceleration.y;
    float v0 = instancePosition.y;
    float limit = centerTop.y;
    float labse = delta * 1.0f;
    float k = (limit + absolutePos.y)/sigma;

    float deltaLimit = (-v0+sqrt(pow(v0,2)-4*a*k))/2*a; //Bhaskara equation

    if(labse > deltaLimit){
        labse = mod(labse, deltaLimit);
    }

    float yf = newPos(labse);

    vec3 newPosition = vec3(absolutePos.x, yf, absolutePos.z); //falta capar delta, en este caso al ser la velocidad inicial muy bajo el valor de sigma practicamente lo anula

    gl_Position = projection * view * vec4(newPosition, 1.0);
    gl_PointSize = pointSize;
}
