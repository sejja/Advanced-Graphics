#ifndef __PREVSTATES__H__
#define __PREVSTATES__H__

#include <iostream>
#include <memory> 
#include "Core/ECSystem/Object.h"
#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/Lights/Light.h"


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

    //LIGHT
	static void SetPrevLight(std::shared_ptr<::Graphics::Primitives::Lights::Light> light) { 
		std::make_shared<::Graphics::Primitives::Lights::Light>(*light);
	}
	static std::shared_ptr<::Graphics::Primitives::Lights::Light> GetPrevLight() { return initLight; }

private:
    static glm::vec3 prevPos;
    static glm::vec3 prevRot;
    static glm::vec3 prevScale;

    static Core::Assets::Asset<Core::Graphics::Texture> prevNormal;
	static Core::Assets::Asset<Core::Graphics::Texture> prevDiffuse;

    static Core::Assets::AssetReference<::Graphics::Primitives::Model> initModel;
    static Core::Assets::AssetReference<Core::Graphics::ShaderProgram> initShader;

	static std::shared_ptr<::Graphics::Primitives::Lights::Light> initLight;


};


#endif
