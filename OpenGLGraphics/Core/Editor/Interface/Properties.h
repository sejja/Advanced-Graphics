#ifndef __PROPERTIES__H__
#define __PROPERTIES__H__

#include <iostream>
#include <glew.h>

class Properties {
public:
    void Render();

private:
    void TransformOptions();
    void CameraOptions();
    void LightingOptions();
    void MaterialsOptions();
    void MeshOptions();




};


#endif 
