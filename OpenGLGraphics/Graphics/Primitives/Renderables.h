//
//	Renderables.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 10/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _RENDERABLES__H_
#define _RENDERABLES__H_

#include "glm.hpp"
#include "Core/ECSystem/Component.h"
#include "Core/Graphics/Pipeline.h"
#include "Graphics/Primitives/Model.h"
#include "Core/Assets/ResourceManager.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/GLBModel.h"

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

		template<Pipeline::GraphicsAPIS E>
		class GLBModelRenderer : public Renderable {};

		template<>
		class GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL> : public Renderable {
#pragma region //Constructor
		public:
			GLBModelRenderer(const std::weak_ptr<Object>& parent) : Renderable(parent) {}
#pragma endregion

#pragma region //Functions
			inline void SetMesh(const Core::Assets::AssetReference<::Graphics::Primitives::GLBModel>& model);
			
			
			inline void SetShaderProgram(const Core::Assets::AssetReference<Graphics::ShaderProgram>& s);

			DONTDISCARD inline Core::Assets::AssetReference<Graphics::ShaderProgram> GetShaderProgram();
			DONTDISCARD inline Core::Assets::AssetReference<::Graphics::Primitives::GLBModel> GetMesh() const;

			void Render() const noexcept override;
			inline void SetProcudesShadow(const bool shadow) noexcept;
			inline bool GetProcudesShadow() const noexcept;

#pragma endregion

#pragma region //Variables
		private:
			Core::Assets::Asset<::Graphics::Primitives::GLBModel> mModel;
			Core::Assets::Asset<Graphics::ShaderProgram> mShaderProgram;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Mesh
		*
		*   Sets the Mesh associated with this renderer
		*/ // ---------------------------------------------------------------------
		void GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL>::SetMesh(const Core::Assets::AssetReference<::Graphics::Primitives::GLBModel>& model) {
			mModel = model.lock();
		}



		// ------------------------------------------------------------------------
		/*! Set Shader Program
		*
		*   Sets the Shader Program associated with this renderer
		*/ // ---------------------------------------------------------------------
		void GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL>::SetShaderProgram(const Core::Assets::AssetReference<Graphics::ShaderProgram>& s) {
			mShaderProgram = s.lock();
		}

		// ------------------------------------------------------------------------
		/*! Get Shader Program
		*
		*   Gets the Shader Program associated with this renderer
		*/ // ---------------------------------------------------------------------
		Core::Assets::AssetReference<Graphics::ShaderProgram> GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL>::GetShaderProgram() {
			return mShaderProgram;
		}

		// ------------------------------------------------------------------------
		/*! Get Mesh
		* 
		* Gets the Mesh associated with this renderer
		*/ // ---------------------------------------------------------------------

		Core::Assets::AssetReference<::Graphics::Primitives::GLBModel> GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL>::GetMesh() const {
			return mModel;
		}


	}
}

#endif