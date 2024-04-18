#include "Decal.h"
#include "Core/ECSystem/Object.h"
#include "gtc/matrix_transform.hpp"
#include "Graphics/Architecture/GeometryDeform.h"

Decal::Decal(std::weak_ptr<Core::Object> parent) :
	Renderable(parent) {
	mModel = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Graphics::Primitives::GLBModel>("Content/Meshes/cube_averaged.obj");
	mShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/GeometryDecal.shader");
	mDiffuse = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/decal_skull_d.png");
	mNormal = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/decal_skull_n.png");
	Graphics::Architecture::GeometryDeform::mDecals.push_back(this);
}

Decal::~Decal() {
	Graphics::Architecture::GeometryDeform::mDecals.erase(std::find(Graphics::Architecture::GeometryDeform::mDecals.begin(),
		Graphics::Architecture::GeometryDeform::mDecals.end(), this));
}

void Decal::Render() const {
    mShader->Get()->Bind();
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
	mShader->Get()->SetShaderUniform("g_world", &matrix);
    mModel->Get()->Draw();
}