//
//	Camera.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 16/04/23
//	Copyright © 2023. All Rights reserved
//

#include "Camera.h"
#include "SDL.h"
#include "Core/Input/InputManager.h"
#include "Core/Singleton.h"
#include "Core/RTTI/RTTI.h"

#include "Core/Editor/Editor.h"


namespace Core {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs the Camera, with its position and subscribes to events
		*/ // ---------------------------------------------------------------------
		Camera::Camera() :
			Position(0, 0, 0), Front(0, 0, -1), Yaw(-90.f), Pitch(0.f), WorldUp(0, 1, 0) {
			static float lastX = 0, lastY = 0;
			static bool firstMouse = true;
			auto& ed = Singleton<Events::EventDispatcher>::Instance();
			ed.Subscribe(*this, Core::Input::InputManager::A_Down(), [this](const Events::Event& event) { Position -= Right; UpdateCameraVectors();});
			ed.Subscribe(*this, Core::Input::InputManager::W_Down(), [this](const Events::Event& event) {Position += Front; UpdateCameraVectors(); });
			ed.Subscribe(*this, Core::Input::InputManager::D_Down(), [this](const Events::Event& event) {Position += Right; UpdateCameraVectors(); });
			ed.Subscribe(*this, Core::Input::InputManager::S_Down(), [this](const Events::Event& event) {Position -= Front; UpdateCameraVectors(); });
			ed.Subscribe(*this, Core::Input::InputManager::MouseMotion(), [this](const Events::Event& event) {
				glm::u16vec2 mouseMotion = (static_cast<const Core::Input::InputManager::MouseMotion&>(event)).mMovement;
				if (firstMouse)
				{
					lastX = mouseMotion.x;
					lastY = mouseMotion.y;
					firstMouse = false;
				}
				
				float xoffset = mouseMotion.x - lastX;
				float yoffset = lastY - mouseMotion.y;
				lastX = mouseMotion.x;
				lastY = mouseMotion.y;

				if(Singleton<Core::Input::InputManager>::Instance().IsKeyDown(VK_LBUTTON)) {
					float sensitivity = 0.1f;
					xoffset *= sensitivity;
					yoffset *= sensitivity;

					Yaw += xoffset;
					Pitch += yoffset;

					if (Pitch > 89.0f)
						Pitch = 89.0f;
					if (Pitch < -89.0f)
						Pitch = -89.0f;

					glm::vec3 direction;
					direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
					direction.y = sin(glm::radians(Pitch));
					direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
					direction = glm::normalize(direction);
					UpdateCameraVectors();
				}
			});

			UpdateCameraVectors();
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Unsubscribes from all the events
		*/ // ---------------------------------------------------------------------
		Camera::~Camera() {
			auto& ed = Singleton<Events::EventDispatcher>::Instance();
			ed.Unsubscribe(*this, Core::Input::InputManager::A_Down());
			ed.Unsubscribe(*this, Core::Input::InputManager::W_Down());
			ed.Unsubscribe(*this, Core::Input::InputManager::D_Down());
			ed.Unsubscribe(*this, Core::Input::InputManager::S_Down());
			ed.Unsubscribe(*this, Core::Input::InputManager::E_Down());
			ed.Unsubscribe(*this, Core::Input::InputManager::Q_Down());
		}

		void Camera::UpdateCameraVectors() {
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			// also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}

		// ------------------------------------------------------------------------
		/*! Get Projection Matrix
		*
		*   Returns the Camera's Projection Matrix
		*/ //----------------------------------------------------------------------
		glm::mat4 Camera::GetProjectionMatrix() const {
			float aspectRatio = Singleton<::Editor>::Instance().GetAspectRatio();
			float fov = *Singleton<::Editor>::Instance().GetFOV();

			return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 10000.f);
		}
	}
}