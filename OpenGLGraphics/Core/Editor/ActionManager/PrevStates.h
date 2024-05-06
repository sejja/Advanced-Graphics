#ifndef __PREVSTATES__H__
#define __PREVSTATES__H__

#include <iostream>
#include <memory> 
#include "Core/ECSystem/Object.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/Lights/Light.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"


class PrevStates {
public:

	//OBJ TRANSFORM
    static void SetPrevPos(glm::vec3 pos) { prevPos = pos; }
    static void SetPrevRot(glm::vec3 rot) { prevRot = rot; }
    static void SetPrevScale(glm::vec3 scale) { prevScale = scale; }

    static glm::vec3 GetPrevPos() { return prevPos; }
    static glm::vec3 GetPrevRot() { return prevRot; }
    static glm::vec3 GetPrevScale() { return prevScale; }

	//MESH
	static void SetPrevNormal(Core::Assets::Asset<Core::Graphics::Texture> normal) { prevNormal = normal; }
	static void SetPrevDiffuse(Core::Assets::Asset<Core::Graphics::Texture> diffuse) { prevDiffuse = diffuse; }

	static Core::Assets::Asset<Core::Graphics::Texture> GetPrevNormal() { return prevNormal; }
	static Core::Assets::Asset<Core::Graphics::Texture> GetPrevDiffuse() { return prevDiffuse; }

    //MODEL 

	static void SetPrevModel(Core::Assets::AssetReference<::Graphics::Primitives::Model> model) { initModel = model; }
	static void SetPrevShader(Core::Assets::AssetReference<Core::Graphics::ShaderProgram> shader) { initShader = shader; }

	static Core::Assets::AssetReference<::Graphics::Primitives::Model> GetPrevModel() { return initModel; }
	static Core::Assets::AssetReference<Core::Graphics::ShaderProgram> GetPrevShader() { return initShader; }


	
	static void SetPrevLight(std::shared_ptr<::Graphics::Primitives::Lights::Light> light) {
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

	static glm::vec3 GetPrevLightPos() { return prevLightPos; }
	static glm::vec3 GetPrevLightColor() { return prevLightColor; }
	static float GetPrevLightRadius() { return prevLightRadius; }
	static float GetPrevLightFallOff() { return prevLightFallOff; }
	static glm::vec3 GetPrevLightDirection() { return prevLightDirection; }
	static float GetPrevLightOuter() { return prevLightOuter; }
	static float GetPrevLightInner() { return prevLightInner; }
	static bool GetPrevLightShadowCaster() { return prevLightShadowCaster; }




    
	


private:
    static glm::vec3 prevPos;
    static glm::vec3 prevRot;
    static glm::vec3 prevScale;

    static Core::Assets::Asset<Core::Graphics::Texture> prevNormal;
	static Core::Assets::Asset<Core::Graphics::Texture> prevDiffuse;

    static Core::Assets::AssetReference<::Graphics::Primitives::Model> initModel;
    static Core::Assets::AssetReference<Core::Graphics::ShaderProgram> initShader;

	//LIGHTS
	static glm::vec3 prevLightPos;
	static glm::vec3 prevLightColor;
	static float prevLightRadius;
	static float prevLightFallOff;
	static glm::vec3 prevLightDirection;
	static float prevLightOuter;
	static float prevLightInner;
	static bool prevLightShadowCaster;




};


#endif
