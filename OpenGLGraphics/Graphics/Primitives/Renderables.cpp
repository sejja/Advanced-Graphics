//
//	Renderables.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 10/04/23
//	Copyright © 2023. All Rights reserved
//

#include "Graphics/Primitives/Renderables.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader.h>
#include <glew.h>

namespace Core {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Renders this  Model
		*/ //----------------------------------------------------------------------
		void ModelRenderer<GraphicsAPIS::OpenGL>::Render() const noexcept {
			Model* const model = mModel->Get();

			model->SetShaderUniforms(*mShaderProgram->Get());
			glBindVertexArray(model->GetHandle());
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(model->GetVertexCount()));
		}

		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs a renderable with it's parent object
		*/ //----------------------------------------------------------------------
		Renderable::Renderable(const std::weak_ptr<Object>& parent) :
			Component(parent) {}
	}
}