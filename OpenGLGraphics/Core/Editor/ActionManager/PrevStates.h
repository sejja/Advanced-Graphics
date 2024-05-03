#ifndef __PREVSTATES__H__
#define __PREVSTATES__H__

#include <iostream>
#include <memory> 
#include "Core/ECSystem/Object.h"
#include "Graphics/Primitives/GLBModel.h"

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




private:
    static glm::vec3 prevPos;
    static glm::vec3 prevRot;
    static glm::vec3 prevScale;

    static Core::Assets::Asset<Core::Graphics::Texture> prevNormal;
	static Core::Assets::Asset<Core::Graphics::Texture> prevDiffuse;

};


#endif
