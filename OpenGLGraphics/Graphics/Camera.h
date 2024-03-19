//
//	Camera.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 16/04/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _CAMERA__H_
#define _CAMERA__H_

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
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
			DONTDISCARD inline glm::mat4 GetProjectionMatrix() const;
			DONTDISCARD glm::vec3 inline GetPosition() const;
			DONTDISCARD inline glm::vec3& GetPositionRef();
			
			DONTDISCARD float GetZNear();
			DONTDISCARD float GetZFar();
			DONTDISCARD float GetZFov();
		#pragma endregion

		#pragma region //Members
		private:
			glm::vec3 mPosition, mTargetPosition;
			const float zNear = 0.f;
			const float zfar = 10000.0f;
			const float fov = 45.f;
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
		/*! Get Projection Matrix
		*
		*   Returns the Camera's Projection Matrix
		*/ //----------------------------------------------------------------------
		glm::mat4 Camera::GetProjectionMatrix() const {
			return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100000.0f);
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

		// ------------------------------------------------------------------------
		/*! Get closest z
		*
		*   Return the minimun distance that is visible
		*/ //----------------------------------------------------------------------
		inline DONTDISCARD float Camera::GetZNear()
		{
			return zNear;
		}

		// ------------------------------------------------------------------------
		/*! Get farest z
		*
		*   Return the maximum distance that is visible
		*/ //----------------------------------------------------------------------
		inline DONTDISCARD float Camera::GetZFar()
		{
			return zfar;
		}

		// ------------------------------------------------------------------------
		/*! Get the fov
		*
		*   Return the maximum distance that is visible
		*/ //----------------------------------------------------------------------
		inline DONTDISCARD float Camera::GetZFov()
		{
			return fov;
		}
		
	}
}

#endif