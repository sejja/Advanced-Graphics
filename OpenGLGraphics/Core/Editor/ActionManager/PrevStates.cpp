#include "PrevStates.h"

// init variables estáticas
glm::vec3 PrevStates::prevPos;
glm::vec3 PrevStates::prevRot;
glm::vec3 PrevStates::prevScale;


Core::Assets::Asset<Core::Graphics::Texture> PrevStates::prevNormal;
Core::Assets::Asset<Core::Graphics::Texture> PrevStates::prevDiffuse;

Core::Assets::AssetReference<::Graphics::Primitives::Model> PrevStates::initModel;
Core::Assets::AssetReference<Core::Graphics::ShaderProgram> PrevStates::initShader;


std::shared_ptr<::Graphics::Primitives::Lights::Light> PrevStates::initLight;