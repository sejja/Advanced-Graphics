//
//	Decal.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _DECAL__H_
#define _DECAL__H_

#include "Core/ECSystem/Component.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/Renderables.h"
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"

namespace Graphics {
	namespace Primitives {
		class Decal : public Core::Graphics::Renderable {
		#pragma region //Constructor & Destructor
		public:
			Decal(const std::weak_ptr<Core::Object>& parent);
			~Decal();
		#pragma endregion

		#pragma region //Methods
			void Render() const;
			void Render(Core::Graphics::ShaderProgram& prog) const;
			DONTDISCARD Core::Assets::AssetReference<Core::Graphics::Texture> GetDiffuse() const noexcept;
			DONTDISCARD Core::Assets::AssetReference<Core::Graphics::Texture> GetNormal() const noexcept;
		#pragma endregion

		#pragma region //Members
		private:
			Core::Assets::Asset<Core::Graphics::Texture> mDiffuse;
			Core::Assets::Asset<Core::Graphics::Texture> mNormal;
		#pragma endregion
		};
	}
}

#endif