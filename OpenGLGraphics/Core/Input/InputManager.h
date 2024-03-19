//
//	InputManager.hpp
//	OpenGL Graphics
//
//	Created by Diego Revilla 14/01/23
//	Copyright � 2024 . All Rights reserved
//

#ifndef _INPUT_MANAGER__H_
#define _INPUT_MANAGER__H_

#include <array>
#include <Windows.h>
#include <vec2.hpp>
#include "CommonDefines.h"
#include "Core/Events/EventDispatcher.h"

namespace Core {
    namespace Input {
        class InputManager {
#pragma region //Declarations
        public:
            using KeyCode = decltype(VK_ACCEPT);

            EVENT_DECL(V_Down);
            EVENT_DECL(Z_Down);
            EVENT_DECL(W_Down);
            EVENT_DECL(A_Down);
            EVENT_DECL(S_Down);
            EVENT_DECL(D_Down);
            EVENT_DECL(Space_Down);
            EVENT_DECL(Shift_Down);
#pragma endregion

#pragma region //Functions
            DONTDISCARD bool inline IsKeyPressed(const KeyCode key) noexcept;
            DONTDISCARD bool inline IsKeyReleased(const KeyCode key) noexcept;
            DONTDISCARD bool inline IsKeyDown(const KeyCode key) const noexcept;
            DONTDISCARD bool inline IsKeyUp(const KeyCode key) const noexcept;
            DONTDISCARD glm::vec2 inline GetMouseMovement() const noexcept;
            DONTDISCARD glm::vec2 inline GetMousePosition() const noexcept;
            void inline SetMousePosition(const glm::u16vec2& pos) const noexcept;
            void SetMousePosition(const unsigned w, const unsigned h) const noexcept;
            void ProcessInput() noexcept;
        private:
            glm::u8vec2 GetWindowCoordinatesMousePosition() const noexcept;
#pragma endregion

#pragma region //Members
            std::array<bool, 101> mKeyboardState;
            glm::u8vec2 mMousePosition;
#pragma endregion
        };

        // ------------------------------------------------------------------------
        /*! Is Key Down
        *
        *   Name says everything
        */ // ---------------------------------------------------------------------
        bool inline InputManager::IsKeyDown(const KeyCode key) const noexcept {
            return (GetAsyncKeyState(key) & (1 << 16));
        }

        // ------------------------------------------------------------------------
        /*! Is Key Up
        *
        *   Name says everything
        */ // ---------------------------------------------------------------------
        bool inline InputManager::IsKeyUp(const KeyCode key) const noexcept {
            return !IsKeyDown(key);
        }

        // ------------------------------------------------------------------------
        /*! Set Mouse Position
        *   Name says everything
        */ // ---------------------------------------------------------------------
        void inline InputManager::SetMousePosition(const glm::u16vec2& pos) const noexcept {
            SetMousePosition(pos.x, pos.y);
        }

        // ------------------------------------------------------------------------
        /*! Get Mouse Position
        *   Name says everything
        */ // ---------------------------------------------------------------------
        glm::vec2 inline InputManager::GetMousePosition() const noexcept {
            return mMousePosition;
        }

        // ------------------------------------------------------------------------
        /*! Is Key Pressed
        *
        *   Name says everything
        */ // ---------------------------------------------------------------------
        bool InputManager::IsKeyPressed(const KeyCode key) noexcept {
            return ((mKeyboardState[key] = IsKeyDown(key)) && !mKeyboardState[key]);
        }

        // ------------------------------------------------------------------------
        /*! Is Key Released
        *
        *   Name says everything
        */ // ---------------------------------------------------------------------
        bool InputManager::IsKeyReleased(const KeyCode key) noexcept {
            return (!(mKeyboardState[key] = IsKeyDown(key)) && mKeyboardState[key]);
        }

        // ------------------------------------------------------------------------
        /*! Get Mouse Movement
        *   Name says everything
        */ // ---------------------------------------------------------------------
        glm::vec2 InputManager::GetMouseMovement() const noexcept {
            return GetWindowCoordinatesMousePosition() - mMousePosition;
        }
    }
}

#endif