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
#include "Core/Singleton.h"





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
			DONTDISCARD  glm::mat4 GetProjectionMatrix() const;
			DONTDISCARD glm::vec3 inline GetPosition() const;
			DONTDISCARD inline glm::vec3& GetPositionRef();
			
			DONTDISCARD float GetZNear();
			DONTDISCARD float GetZFar();
			DONTDISCARD float GetZFov();
			void UpdateCameraVectors();
		#pragma endregion

		#pragma region //Members
			constexpr static float cnearPlane = 0.1f;
			constexpr static float cfarPlane = 10000.f;

		private:
			// camera Attributes
			glm::vec3 Position;
			glm::vec3 Front;
			glm::vec3 Up;
			glm::vec3 Right;
			glm::vec3 WorldUp;
			// euler Angles
			float Yaw;
			float Pitch;
			// camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;
		#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get View Matrix
		*
		*   Returns the Camera's View Matrix
		*/ //----------------------------------------------------------------------
		glm::mat4 Camera::GetViewMatrix() const {
			return std::move(glm::lookAt(Position, Position + Front, Up));
		}

		// ------------------------------------------------------------------------
		/*! Get Position
		*
		*   Returns the Camera's Position
		*/ //----------------------------------------------------------------------
		glm::vec3 inline Camera::GetPosition() const {
			return Position;
		}

		// ------------------------------------------------------------------------
		/*! Get Position Reference
		*
		*   Returns the Camera's Position, as a Reference
		*/ //----------------------------------------------------------------------
		inline glm::vec3& Camera::GetPositionRef() {
			return Position;
		}

		// ------------------------------------------------------------------------
		/*! Get closest z
		*
		*   Return the minimun distance that is visible
		*/ //----------------------------------------------------------------------
		inline DONTDISCARD float Camera::GetZNear()
		{
			return 0.1f;
		}

		// ------------------------------------------------------------------------
		/*! Get farest z
		*
		*   Return the maximum distance that is visible
		*/ //----------------------------------------------------------------------
		inline DONTDISCARD float Camera::GetZFar()
		{
			return 10000.f;
		}

		// ------------------------------------------------------------------------
		/*! Get the fov
		*
		*   Return the maximum distance that is visible
		*/ //----------------------------------------------------------------------
		inline DONTDISCARD float Camera::GetZFov()
		{
			return 45.f;
		}
		
	}
}

#endif