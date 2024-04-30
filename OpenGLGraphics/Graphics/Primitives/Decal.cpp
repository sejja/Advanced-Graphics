//
//	Decal.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024. All Rights reserved
//

#include "Decal.h"
#include "Core/ECSystem/Object.h"
#include "gtc/matrix_transform.hpp"
#include "Graphics/Architecture/GeometryDeform.h"

namespace Graphics {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*  Constructs a decal with a parent objects
		*/ //----------------------------------------------------------------------
		Decal::Decal(const std::weak_ptr<Core::Object>& parent) :
			Renderable(parent), mDiffuse(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/decal_skull_d.png")),
			mNormal(Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/decal_skull_n.png")) {
			Graphics::Architecture::GeometryDeform::AddDecal(this);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*  Removes the decal from the pipeline
		*/ //----------------------------------------------------------------------
		Decal::~Decal() {
			Graphics::Architecture::GeometryDeform::RemoveDecal(this);
		}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*  NOT USED FUNCTION
		*/ //----------------------------------------------------------------------
		void Decal::Render() const {}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*  Renders the decal, 
		*/ //----------------------------------------------------------------------
		void Decal::Render(Core::Graphics::ShaderProgram& prog) const {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mDiffuse->Get()->GetTextureHandle());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mNormal->Get()->GetTextureHandle());
			const std::shared_ptr<Core::Object> parent = GetParent().lock();
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition()) *
				glm::rotate(glm::mat4(1.0f), parent->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::rotate(glm::mat4(1.0f), parent->GetRotation().y, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), parent->GetRotation().x, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::scale(glm::mat4(1.0f), parent->GetScale());
			prog.SetShaderUniform("g_world", &matrix);
		}
	}
}