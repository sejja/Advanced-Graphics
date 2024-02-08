//
//	InputManager.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla 14/01/23
//	Copyright © 2021 . All Rights reserved
//

#include "InputManager.h"
#include "Singleton.h"

constexpr int c_sensibility = 5000;

namespace Core {
	// ------------------------------------------------------------------------
	/*! Instance
	*
	*   Returns an instance of the Input Manager
	*/ // ---------------------------------------------------------------------
	InputManager& InputManager::Instance() noexcept {
		static InputManager inptmgr;

		return inptmgr;
	}

	// ------------------------------------------------------------------------
	/*! Is Key Pressed
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::IsKeyPressed(const KeyCode key) noexcept {
		const bool previousState = mKeyboardState[key];

		return ((mKeyboardState[key] = IsKeyDown(key)) && !previousState);
	}

	// ------------------------------------------------------------------------
	/*! Is Key Released
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::IsKeyReleased(const KeyCode key) noexcept {
		const bool previousState = mKeyboardState[key];

		return (!(mKeyboardState[key] = IsKeyDown(key)) && previousState);
	}

	// ------------------------------------------------------------------------
	/*! Get Mouse Movement
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	glm::vec2 InputManager::GetMouseMovement() const noexcept {
		glm::vec2 pos = GetWindowCoordinatesMousePosition();
		pos -= mMousePositon;
		return pos;
	}

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
		mMousePositon = GetWindowCoordinatesMousePosition();

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
	glm::vec2 InputManager::GetWindowCoordinatesMousePosition() const noexcept {
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(GetActiveWindow(), &p);

		return  { static_cast<float>(p.x), static_cast<float>(p.y) };
	}
}