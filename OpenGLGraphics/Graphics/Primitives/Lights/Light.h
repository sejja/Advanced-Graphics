//
//	Light.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _LIGHT__H_
#define _LIGHT__H_

#include "glm.hpp"
#include "Core/ECSystem/Component.h"

namespace Graphics {
	namespace Primitives {
		namespace Lights {
			class Light : public Core::Component {
			#pragma region //Declarations
			public:
				struct BackedLightData {
				#pragma region //Contructor
					BackedLightData();
				#pragma
				#pragma region //Methods
					virtual float CalculateSphereOfInfluence() const = 0;
				#pragma endregion

				#pragma region //Members
					glm::vec3 mPosition;
					glm::vec3 mColor;
					bool mShadowCaster;
				#pragma endregion
				};
			#pragma endregion

			#pragma region //Constructor & Destructor
				Light(const std::weak_ptr<Core::Object>& parent);
			#pragma endregion

			#pragma region //Methods
				DONTDISCARD glm::vec3 inline GetPosition() const noexcept;
				void SetPosition(const glm::vec3& position) noexcept;
				DONTDISCARD glm::vec3 inline GetColor() const noexcept;
				void inline SetColor(const glm::vec3& color) noexcept;
			#pragma endregion

			#pragma region //Members
			protected:
				std::shared_ptr<BackedLightData> mData;
			#pragma endregion
			};

			// ------------------------------------------------------------------------
			/*! Get Color
			*
			*   Returns the color of the light
			*/ //----------------------------------------------------------------------
			glm::vec3 Light::GetColor() const noexcept {
				return mData->mColor;
			}

			// ------------------------------------------------------------------------
			/*! Set Color
			*
			*   Sets the color of the light
			*/ //----------------------------------------------------------------------
			void Light::SetColor(const glm::vec3& color) noexcept {
				mData->mColor = color;
			}

			// ------------------------------------------------------------------------
			/*! Get Position
			*
			*   Gets the position of the light, relative to the parent
			*/ //----------------------------------------------------------------------
			glm::vec3 Light::GetPosition() const noexcept {
				return GetParent().expired() ? glm::vec3() : mData->mPosition - GetParent().lock()->GetPosition();
			}
		}
	}
}

#endif