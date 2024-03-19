//
//	InputManager.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla 20/03/25
//	Copyright © 2024. All Rights reserved
//

#include "InputManager.h"
#include "Core/Singleton.h"

namespace Core {
	namespace Input {
		// ------------------------------------------------------------------------
	/*! Set Mouse Position
	*   Name says everything
	*/ // ---------------------------------------------------------------------
		void InputManager::SetMousePosition(const unsigned w, const unsigned h) const noexcept {
			POINT p{ static_cast<LONG>(w), static_cast<LONG>(h) };
			ClientToScreen(GetActiveWindow(), &p);
			SetCursorPos(p.x, p.y);
		}

		// ------------------------------------------------------------------------
		/*! Process Input
		*
		*   Updates the Input Information for the application
		*/ // ---------------------------------------------------------------------
		void InputManager::ProcessInput() noexcept {
			for (KeyCode i = 0; i < 101; i++) mKeyboardState[i] = IsKeyDown(i);
			mMousePosition = GetWindowCoordinatesMousePosition();

			if (IsKeyDown('A')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(A_Down());
			if (IsKeyDown('Z')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(Z_Down());
			if (IsKeyDown('W')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(W_Down());
			if (IsKeyDown('V')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(V_Down());
			if (IsKeyDown('S')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(S_Down());
			if (IsKeyDown('D')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(D_Down());
			if (IsKeyDown('Q')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(Space_Down());
			if (IsKeyDown('R')) Singleton<Events::EventDispatcher>::Instance().TriggerEvent(Shift_Down());
		}

		// ------------------------------------------------------------------------
		/*! Get Window-Coordinates Mouse Position
		*
		*   Win32 Wrapper
		*/ // ---------------------------------------------------------------------
		glm::u8vec2 InputManager::GetWindowCoordinatesMousePosition() const noexcept {
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(GetActiveWindow(), &p);

			return  { p.x, p.y };
		}
	}
}