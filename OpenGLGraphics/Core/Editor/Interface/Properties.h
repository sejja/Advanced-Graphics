#ifndef __PROPERTIES__H__
#define __PROPERTIES__H__

#include <iostream>
#include <glew.h>
#include "Graphics/OpenGLPipeline.h"




class Properties {
public:
    void Render(Core::Graphics::OpenGLPipeline& pipeline);

    bool isFocused() { return focused; }

private:
    void TransformOptions();
    void LightTransform();
    void ParticleTransform();
    void CameraOptions();
    void LightingOptions();
    void LightTypeOptions();
    void MaterialsOptions();
    void MeshOptions();
    void ShaderOptions(Core::Graphics::OpenGLPipeline& pipeline);
    void FireSize();
    void UpdateLightCompsPos(std::shared_ptr<Core::Object> obj);
    void DecalOptions();
    void TransformGuizmoTypeSelect();
    void applyLockResize(glm::vec3& prevVec, glm::vec3& curVec);


    void selectedObjectTree();
    void objectOutliner(Core::Graphics::OpenGLPipeline& pipeline);

    bool focused = false;


    bool* axisLockS = new bool(true);




};


#endif 
