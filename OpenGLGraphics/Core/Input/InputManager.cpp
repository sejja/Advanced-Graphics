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
			using Events::EventDispatcher;
			EventDispatcher& dispatcher = Singleton<EventDispatcher>::Instance();

			for (KeyCode i = 0; i < 101; i++) mKeyboardState[i] = IsKeyDown(i);
			mMousePosition = GetWindowCoordinatesMousePosition();

			if (IsKeyDown('A')) dispatcher.TriggerEvent(A_Down());
			if (IsKeyDown('B')) dispatcher.TriggerEvent(B_Down());
			if (IsKeyDown('C')) dispatcher.TriggerEvent(C_Down());
			if (IsKeyDown('D')) dispatcher.TriggerEvent(D_Down());
			if (IsKeyDown('E')) dispatcher.TriggerEvent(E_Down());
			if (IsKeyDown('F')) dispatcher.TriggerEvent(F_Down());
			if (IsKeyDown('G')) dispatcher.TriggerEvent(G_Down());
			if (IsKeyDown('H')) dispatcher.TriggerEvent(H_Down());
			if (IsKeyDown('I')) dispatcher.TriggerEvent(I_Down());
			if (IsKeyDown('J')) dispatcher.TriggerEvent(J_Down());
			if (IsKeyDown('K')) dispatcher.TriggerEvent(K_Down());
			if (IsKeyDown('L')) dispatcher.TriggerEvent(L_Down());
			if (IsKeyDown('M')) dispatcher.TriggerEvent(M_Down());
			if (IsKeyDown('N')) dispatcher.TriggerEvent(N_Down());
			if (IsKeyDown('O')) dispatcher.TriggerEvent(O_Down());
			if (IsKeyDown('P')) dispatcher.TriggerEvent(P_Down());
			if (IsKeyDown('Q')) dispatcher.TriggerEvent(Q_Down());
			if (IsKeyDown('R')) dispatcher.TriggerEvent(R_Down());
			if (IsKeyDown('S')) dispatcher.TriggerEvent(S_Down());
			if (IsKeyDown('T')) dispatcher.TriggerEvent(T_Down());
			if (IsKeyDown('U')) dispatcher.TriggerEvent(U_Down());
			if (IsKeyDown('V')) dispatcher.TriggerEvent(V_Down());
			if (IsKeyDown('W')) dispatcher.TriggerEvent(W_Down());
			if (IsKeyDown('X')) dispatcher.TriggerEvent(X_Down());
			if (IsKeyDown('Y')) dispatcher.TriggerEvent(Y_Down());
			if (IsKeyDown('Z')) dispatcher.TriggerEvent(Z_Down());
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