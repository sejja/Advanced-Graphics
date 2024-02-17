#ifndef __PROPERTIES__H__
#define __PROPERTIES__H__

#include <iostream>
#include <glew.h>

class Properties {
public:
    void Render();

    void RenderOutliner();

    void RenderProperties();

    GLuint LoadTexture(const char* filename);


};


#endif 

