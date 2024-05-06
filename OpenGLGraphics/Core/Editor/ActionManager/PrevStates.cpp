#include "PrevStates.h"

// init variables estáticas
glm::vec3 PrevStates::prevPos;
glm::vec3 PrevStates::prevRot;
glm::vec3 PrevStates::prevScale;


Core::Assets::Asset<Core::Graphics::Texture> PrevStates::prevNormal;
Core::Assets::Asset<Core::Graphics::Texture> PrevStates::prevDiffuse;

Core::Assets::AssetReference<::Graphics::Primitives::Model> PrevStates::initModel;
Core::Assets::AssetReference<Core::Graphics::ShaderProgram> PrevStates::initShader;


glm::vec3 PrevStates::prevLightPos;
glm::vec3 PrevStates::prevLightColor;
float PrevStates::prevLightRadius;
float PrevStates::prevLightFallOff;
glm::vec3 PrevStates::prevLightDirection;
float PrevStates::prevLightOuter;
float PrevStates::prevLightInner;
bool PrevStates::prevLightShadowCaster;

