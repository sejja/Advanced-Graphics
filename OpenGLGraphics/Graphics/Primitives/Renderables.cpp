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
		void GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL>::Render() const noexcept {
			mModel->Get()->Draw();
		}

		::Graphics::Primitives::Model* GLBModelRenderer<Pipeline::GraphicsAPIS::OpenGL>::getModel()
		{
			return mModel.get()->Get();
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