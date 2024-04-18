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
				~Light();
			#pragma endregion

			#pragma region //Methods
				DONTDISCARD glm::vec3 GetPosition() const noexcept;
				void SetPosition(const glm::vec3& position) noexcept;
				DONTDISCARD glm::vec3 GetColor() const noexcept;
				void SetColor(const glm::vec3& color) noexcept;
				void SetData(const std::weak_ptr<BackedLightData>& data);
				std::weak_ptr<BackedLightData> GetData() const noexcept;
			#pragma endregion

			#pragma region //Members
			protected:
				std::shared_ptr<BackedLightData> mData;
			#pragma endregion
			};
		}
	}
}

#endif