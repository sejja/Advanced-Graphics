#include "LightAction.h"

LightAction::LightAction(std::shared_ptr<::Graphics::Primitives::Lights::Light> light){
	this->mLight = light;
	prevLightPos = PrevStates::GetPrevLightPos();
	prevLightColor = PrevStates::GetPrevLightColor();
	curLightPos = light->GetPosition();
	curLightColor = light->GetColor();

	if (auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::PointLight>(light)) {
		curLightRadius = pointLight->GetRadius();
		curLightFallOff = pointLight->GetFallOff();
		prevLightRadius = PrevStates::GetPrevLightRadius();
		prevLightFallOff = PrevStates::GetPrevLightFallOff();
		prevFireParams = PrevStates::GetPrevFireParams();
		prevIsFireLight = PrevStates::GetPrevIsFireLight();
	}
	else if (auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::SpotLight>(light)) {
		curLightRadius = spotLight->GetRadius();
		curLightFallOff = spotLight->GetFallOff();
		curLightDirection = spotLight->GetDirection();
		curLightOuter = spotLight->GetOutter();
		curLightInner = spotLight->GetInner();
		curLightShadowCaster = spotLight->GetShadowCasting();

		prevLightRadius = PrevStates::GetPrevLightRadius();
		prevLightFallOff = PrevStates::GetPrevLightFallOff();
		prevLightDirection = PrevStates::GetPrevLightDirection();
		prevLightOuter = PrevStates::GetPrevLightOuter();
		prevLightInner = PrevStates::GetPrevLightInner();
		prevLightShadowCaster = PrevStates::GetPrevLightShadowCaster();
	}
	else if (auto dirLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(light)) {
		curLightDirection = dirLight->GetDirection();
		prevLightDirection = PrevStates::GetPrevLightDirection();
	}

}


void LightAction::Undo() {
	mLight->SetColor(prevLightColor);
	mLight->SetPosition(prevLightPos,mLight->GetParent().lock()->GetPosition());
	if (auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::PointLight>(mLight)) {
		pointLight->SetRadius(prevLightRadius);
		pointLight->SetFallOff(prevLightFallOff);
	}
	else if (auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::SpotLight>(mLight)) {
		spotLight->SetRadius(prevLightRadius);
		spotLight->SetFallOff(prevLightFallOff);
		spotLight->SetDirection(prevLightDirection);
		spotLight->SetOuter(prevLightOuter);
		spotLight->SetInner(prevLightInner);
		spotLight->SetShadowCaster(prevLightShadowCaster);
	}
	else if (auto dirLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(mLight)) {
		dirLight->SetDirection(prevLightDirection);
	}
}

void LightAction::Redo() {
	mLight->SetColor(curLightColor);
	mLight->SetPosition(curLightPos, mLight->GetParent().lock()->GetPosition());
	if (auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::PointLight>(mLight)) {
		pointLight->SetRadius(curLightRadius);
		pointLight->SetFallOff(curLightFallOff);
	}
	else if (auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::SpotLight>(mLight)) {
		spotLight->SetRadius(curLightRadius);
		spotLight->SetFallOff(curLightFallOff);
		spotLight->SetDirection(curLightDirection);
		spotLight->SetOuter(curLightOuter);
		spotLight->SetInner(curLightInner);
		spotLight->SetShadowCaster(curLightShadowCaster);
	}
	else if (auto dirLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(mLight)) {
		dirLight->SetDirection(curLightDirection);
	}
}
