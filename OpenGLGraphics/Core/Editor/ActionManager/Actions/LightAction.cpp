#include "LightAction.h"

LightAction::LightAction(std::shared_ptr<::Graphics::Primitives::Lights::Light> light){
	this->mLight = light;
	this->mLightCp = PrevStates::GetPrevLight();
}


void LightAction::Undo() {
    if (auto pointLightCP = std::dynamic_pointer_cast<::Graphics::Primitives::PointLight>(mLightCp)) {
		auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::PointLight>(mLight);
		pointLight->SetRadius(pointLightCP->GetRadius());
		pointLight->SetFallOff(pointLightCP->GetFallOff());
    }
	else if (auto spotLightCP = std::dynamic_pointer_cast<::Graphics::Primitives::SpotLight>(mLightCp)) {
		auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::SpotLight>(mLight);
		spotLight->SetDirection(spotLightCP->GetDirection());
		spotLight->SetRadius(spotLightCP->GetRadius());
		spotLight->SetFallOff(spotLightCP->GetFallOff());
		spotLight->SetOuter(spotLightCP->GetOutter());
		spotLight->SetInner(spotLightCP->GetInner());
		spotLight->SetShadowCaster(spotLightCP->GetShadowCasting());
	}
	else if (auto directionalLightCP = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(mLightCp)) {
		auto directionalLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(mLight);
		directionalLight->SetDirection(directionalLightCP->GetDirection());

	}
	else {
		std::cout << "Error en LightAction::Undo()" << std::endl;
	}

	//mLight->SetColor(mLightCp->GetColor()); ?? SE ROMPE ???





}

void LightAction::Redo() {
    // Implementa la lógica para rehacer aquí
}
