//
//	Camera.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 16/04/23
//	Copyright © 2023. All Rights reserved
//

#include "Camera.h"
#include "SDL.h"
#include "../Core/InputManager.h"
#include "Core/Singleton.h"
#include "Core/RTTI.h"

namespace Core {
	namespace Primitives {
		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   Constructs the Camera, with its position and subscribes to events
		*/ // ---------------------------------------------------------------------
		Camera::Camera() :
			mPosition(0, 0, 100), mTargetPosition(0, 0, 0) {
			auto& ed = Singleton<Events::EventDispatcher>::Instance();
			ed.Subscribe(*this, Core::InputManager::A_Down(), [this](const Events::Event& event) {mPosition.x -= 1; });
			ed.Subscribe(*this, Core::InputManager::W_Down(), [this](const Events::Event& event) {mPosition.z -= 1; });
			ed.Subscribe(*this, Core::InputManager::D_Down(), [this](const Events::Event& event) {mPosition.x += 1; });
			ed.Subscribe(*this, Core::InputManager::S_Down(), [this](const Events::Event& event) {mPosition.z += 1; });
			ed.Subscribe(*this, Core::InputManager::Shift_Down(), [this](const Events::Event& event) {mPosition.y -= 1; });
			ed.Subscribe(*this, Core::InputManager::Space_Down(), [this](const Events::Event& event) {mPosition.y += 1; });
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Unsubscribes from all the events
		*/ // ---------------------------------------------------------------------
		Camera::~Camera() {
			auto& ed = Singleton<Events::EventDispatcher>::Instance();
			ed.Unsubscribe(*this, Core::InputManager::A_Down());
			ed.Unsubscribe(*this, Core::InputManager::W_Down());
			ed.Unsubscribe(*this, Core::InputManager::D_Down());
			ed.Unsubscribe(*this, Core::InputManager::S_Down());
			ed.Unsubscribe(*this, Core::InputManager::Shift_Down());
			ed.Unsubscribe(*this, Core::InputManager::Space_Down());
		}
	}
}