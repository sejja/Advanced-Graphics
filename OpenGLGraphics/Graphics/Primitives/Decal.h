#pragma once
#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/Renderables.h"
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/GLBModel.h"

class Decal : public Core::Graphics::Renderable {
public:
	Decal(std::weak_ptr<Core::Object> parent);
	~Decal();
	void Render() const override;

	Core::Assets::Asset<Graphics::Primitives::GLBModel> mModel;
	Core::Assets::Asset<Core::Graphics::ShaderProgram> mShader;
	Core::Assets::Asset<Core::Graphics::Texture> mDiffuse;
	Core::Assets::Asset<Core::Graphics::Texture> mNormal;
};