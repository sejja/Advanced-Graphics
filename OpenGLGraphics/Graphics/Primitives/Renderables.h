//
//	Renderables.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 10/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _RENDERABLES__H_
#define _RENDERABLES__H_

#include <glm/glm.hpp>
#include "Core/ECSystem/Component.h"
#include "Core/Pipeline.h"
#include "Graphics/Primitives/Model.h"
#include "Core/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"

namespace Core {
	namespace Graphics {
		struct Vertex {
#pragma region //Variables
			glm::vec3 mPosition;
			glm::vec2 mUVs;
#pragma endregion
		};

		class Renderable : public Component {
#pragma region //Constructor
		public:
			explicit Renderable(const std::weak_ptr<Object>& parent);
#pragma endregion

#pragma region //Function
			virtual void Render() const = 0;
#pragma endregion
		};

		template<GraphicsAPIS E>
		class ModelRenderer : public Renderable {};

		template<>
		class ModelRenderer<GraphicsAPIS::OpenGL> : public Renderable {
#pragma region //Constructor
		public:
			ModelRenderer(const std::weak_ptr<Object>& parent) : Renderable(parent) {}
#pragma endregion

#pragma region //Functions
			inline void SetMesh(const AssetReference<Model>& model);
			inline void SetShaderProgram(const AssetReference<Graphics::ShaderProgram>& s);
			DONTDISCARD inline AssetReference<Graphics::ShaderProgram> GetShaderProgram();
			void Render() const noexcept override;
			inline void SetProcudesShadow(const bool shadow) noexcept;
			inline bool GetProcudesShadow() const noexcept;
#pragma endregion

#pragma region //Variables
		private:
			Asset<Model> mModel;
			Asset<Graphics::ShaderProgram> mShaderProgram;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Mesh
		*
		*   Sets the Mesh associated with this renderer
		*/ // ---------------------------------------------------------------------
		void ModelRenderer<GraphicsAPIS::OpenGL>::SetMesh(const AssetReference<Model>& model) {
			mModel = model.lock();
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Program
		*
		*   Sets the Shader Program associated with this renderer
		*/ // ---------------------------------------------------------------------
		void ModelRenderer<GraphicsAPIS::OpenGL>::SetShaderProgram(const AssetReference<Graphics::ShaderProgram>& s) {
			mShaderProgram = s.lock();
		}

		// ------------------------------------------------------------------------
		/*! Get Shader Program
		*
		*   Gets the Shader Program associated with this renderer
		*/ // ---------------------------------------------------------------------
		AssetReference<Graphics::ShaderProgram> ModelRenderer<GraphicsAPIS::OpenGL>::GetShaderProgram() {
			return mShaderProgram;
		}
	}
}

#endif