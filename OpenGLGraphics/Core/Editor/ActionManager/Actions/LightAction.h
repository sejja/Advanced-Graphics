#ifndef __LIGHTACTION__H__
#define __LIGHTACTION__H__

#include "../Action.h"
#include <memory>
#include "Graphics/Primitives/Lights/Light.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"

class LightAction : public Action {
public:
    LightAction(std::shared_ptr<::Graphics::Primitives::Lights::Light> light);

    virtual void Undo() override;
    virtual void Redo() override;

private:
    std::shared_ptr<::Graphics::Primitives::Lights::Light> mLight;
    std::shared_ptr<::Graphics::Primitives::Lights::Light> mLightCp;
};

#endif