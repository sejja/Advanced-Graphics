#pragma once
#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/Assets/ResourceManager.h"

class Decal : public Core::Component {
public:
	Decal(std::weak_ptr<Core::Object> parent);
	Core::Assets::Asset<Core::Graphics::Texture> mDiffuse;
	Core::Assets::Asset<Core::Graphics::Texture> mNormal;
};