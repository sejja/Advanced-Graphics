#include "LightAction.h"

LightAction::LightAction(std::shared_ptr<::Graphics::Primitives::Lights::Light> light){
	this->mLight = light;

	if (auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::PointLight>(light)) {
		prevLightPos = pointLight->GetPosition();
		prevLightColor = pointLight->GetColor();
		prevLightRadius = pointLight->GetRadius();
		prevLightFallOff = pointLight->GetFallOff();
	}
	else if (auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::SpotLight>(light)) {
		prevLightPos = spotLight->GetPosition();
		prevLightColor = spotLight->GetColor();
		prevLightRadius = spotLight->GetRadius();
		prevLightFallOff = spotLight->GetFallOff();
		prevLightDirection = spotLight->GetDirection();
		prevLightOuter = spotLight->GetOutter();
		prevLightInner = spotLight->GetInner();
		prevLightShadowCaster = spotLight->GetShadowCasting();
	}
	else if (auto dirLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(light)) {
		prevLightPos = dirLight->GetPosition();
		prevLightColor = dirLight->GetColor();
		prevLightDirection = dirLight->GetDirection();
	}

}


void LightAction::Undo() {
	if (auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::PointLight>(mLight)) {
		//pointLight->SetPosition(prevLightPos);
		pointLight->SetColor(prevLightColor);
		pointLight->SetRadius(prevLightRadius);
		pointLight->SetFallOff(prevLightFallOff);
	}
	else if (auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::SpotLight>(mLight)) {
		//spotLight->SetPosition(prevLightPos);
		spotLight->SetColor(prevLightColor);
		spotLight->SetRadius(prevLightRadius);
		spotLight->SetFallOff(prevLightFallOff);
		spotLight->SetDirection(prevLightDirection);
		spotLight->SetOuter(prevLightOuter);
		spotLight->SetInner(prevLightInner);
		spotLight->SetShadowCaster(prevLightShadowCaster);
	}
	else if (auto dirLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(mLight)) {
		//dirLight->SetPosition(prevLightPos);
		dirLight->SetColor(prevLightColor);
		dirLight->SetDirection(prevLightDirection);
	}
}

void LightAction::Redo() {

    
}
