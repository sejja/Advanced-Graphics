#include "Decal.h"

Decal::Decal(std::weak_ptr<Core::Object> parent) :
	Component(parent) {
	mDiffuse = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/decal_skull_d.png");
	mNormal = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/decal_skull_n.png");
}
