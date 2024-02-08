//
//	Camera.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 16/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _CAMERA__H_
#define _CAMERA__H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CommonDefines.h"
#include "Core/Events/EventDispatcher.h"

namespace Core {
	namespace Primitives {
		class Camera : Events::Listener {
		#pragma region //Constructor & Destructor
		public:
			Camera();
			~Camera();
		#pragma endregion

		#pragma region //Functions
			DONTDISCARD inline glm::mat4 GetViewMatrix() const;
			void HandleEvent(const Events::Event& event) override;
			DONTDISCARD glm::vec3 inline GetPosition() const;
			DONTDISCARD inline glm::vec3& GetPositionRef();
		#pragma endregion

		#pragma region //Members
		private:
			glm::vec3 mPosition, mTargetPosition;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get View Matrix
		*
		*   Returns the Camera's View Matrix
		*/ //----------------------------------------------------------------------
		glm::mat4 Camera::GetViewMatrix() const {
			return std::move(glm::lookAt(mPosition, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));
		}

		// ------------------------------------------------------------------------
		/*! Get Position
		*
		*   Returns the Camera's Position
		*/ //----------------------------------------------------------------------
		glm::vec3 inline Camera::GetPosition() const {
			return mPosition;
		}

		// ------------------------------------------------------------------------
		/*! Get Position Reference
		*
		*   Returns the Camera's Position, as a Reference
		*/ //----------------------------------------------------------------------
		inline glm::vec3& Camera::GetPositionRef() {
			return mPosition;
		}
	}
}

#endif