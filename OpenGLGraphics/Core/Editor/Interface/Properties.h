#ifndef __PROPERTIES__H__
#define __PROPERTIES__H__

#include <iostream>
#include <glew.h>
#include "Graphics/OpenGLPipeline.h"




class Properties {
public:
    void Render(Core::Graphics::OpenGLPipeline& pipeline);

private:
    void TransformOptions();
    void LightTransform();
    void ParticleTransform();
    void CameraOptions();
    void LightingOptions();
    void MaterialsOptions();
    void MeshOptions();
    void ShaderOptions(Core::Graphics::OpenGLPipeline& pipeline);
    void FireSize();
    void UpdateLightCompsPos(std::shared_ptr<Core::Object> obj);


    void selectedObjectTree();
    void objectOutliner();




};


#endif 
